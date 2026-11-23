#include <omnetpp.h>
#include <deque>
#include "../rdt/RdtPacket_m.h"

using namespace omnetpp;

class RdtClient : public cSimpleModule
{
  private:
    // Parameters
    int maxMsgSize;
    simtime_t requestTime;

    // State variables
    int expectedSeqNum;
    int totalBytesReceived;
    bool transferComplete;
    bool transferStarted;

    // Send buffer for ACKs
    std::deque<RdtPacket*> sendBuffer;
    cMessage *sendEvent;
    cMessage *requestTimer;

    // Signals
    simsignal_t dataPkRcvdSignal;
    simsignal_t ackPkSentSignal;
    simsignal_t corruptedPkDropSignal;
    simsignal_t dataDelaySignal;

  protected:
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
    virtual void finish() override;

    void sendRequest();
    void queueAck(int ackNum);
    void processDataPacket(RdtPacket *pkt);
    void trySendFromBuffer();
    bool isChannelIdle();
};

Define_Module(RdtClient);

void RdtClient::initialize()
{
    maxMsgSize = par("maxMsgSize");
    requestTime = par("requestTime");

    expectedSeqNum = 0;
    totalBytesReceived = 0;
    transferComplete = false;
    transferStarted = false;

    sendEvent = new cMessage("sendEvent");
    requestTimer = new cMessage("requestTimeout");

    // Register signals
    dataPkRcvdSignal = registerSignal("dataPkRcvd");
    ackPkSentSignal = registerSignal("ackPkSent");
    corruptedPkDropSignal = registerSignal("corruptedPkDrop");
    dataDelaySignal = registerSignal("dataDelay");

    // Schedule request
    scheduleAt(requestTime, new cMessage("sendRequest"));

    EV << "Client initialized\n";
}

void RdtClient::handleMessage(cMessage *msg)
{
    if (msg->isSelfMessage()) {
        if (strcmp(msg->getName(), "sendRequest") == 0) {
            sendRequest();
            delete msg;
        } else if (msg == sendEvent) {
            trySendFromBuffer();
        } else if (msg == requestTimer) {
            // Request timeout - resend request
            EV << "Client: Request timeout, retrying\n";
            sendRequest();
        }
    }
    else {
        RdtPacket *pkt = check_and_cast<RdtPacket*>(msg);

        // Check for bit errors
        if (pkt->hasBitError()) {
            EV << "Client: Corrupted packet dropped\n";
            emit(corruptedPkDropSignal, 1L);
            delete pkt;
            return;
        }

        // If we receive any valid packet, transfer has started
        if (!transferStarted) {
            transferStarted = true;
            // Cancel request timer
            if (requestTimer->isScheduled()) {
                cancelEvent(requestTimer);
            }
        }

        // Handle FIN
        if (pkt->isFin()) {
            EV << "Client: FIN received, transfer complete\n";
            transferComplete = true;
            // ACK the FIN
            if (expectedSeqNum > 0) {
                queueAck(expectedSeqNum - 1);
            }
            delete pkt;
            return;
        }

        // Handle data packet
        if (pkt->isData()) {
            processDataPacket(pkt);
        }

        delete pkt;
    }
}

void RdtClient::sendRequest()
{
    RdtPacket *req = new RdtPacket("REQUEST");
    req->setIsRequest(true);
    req->setByteLength(64);

    EV << "Client: Sending request\n";

    sendBuffer.push_back(req);
    trySendFromBuffer();

    // Set timer to retransmit request if no response
    if (!transferStarted) {
        if (requestTimer->isScheduled()) {
            cancelEvent(requestTimer);
        }
        scheduleAt(simTime() + 1.0, requestTimer);  // 1 second timeout
    }
}

void RdtClient::queueAck(int ackNum)
{
    RdtPacket *ack = new RdtPacket("ACK");
    ack->setIsAck(true);
    ack->setAckNum(ackNum);
    ack->setByteLength(64);

    EV << "Client: Queuing ACK for seqNum=" << ackNum << "\n";
    emit(ackPkSentSignal, 1L);

    sendBuffer.push_back(ack);
    trySendFromBuffer();
}

void RdtClient::processDataPacket(RdtPacket *pkt)
{
    int seqNum = pkt->getSeqNum();

    EV << "Client: Data received seqNum=" << seqNum
       << " expected=" << expectedSeqNum << "\n";

    if (seqNum == expectedSeqNum) {
        // In-order packet - accept
        emit(dataPkRcvdSignal, 1L);

        // Measure delay
        simtime_t delay = simTime() - pkt->getCreationTime();
        emit(dataDelaySignal, delay);

        // Deliver
        int payload = pkt->getPayloadBytes();
        totalBytesReceived += payload;

        EV << "Client: Delivered " << payload << " bytes, total="
           << totalBytesReceived << "\n";

        // Advance window
        expectedSeqNum++;

        // Send ACK for this packet
        queueAck(seqNum);
    }
    else if (seqNum < expectedSeqNum) {
        // Duplicate - send duplicate ACK
        EV << "Client: Duplicate packet, sending duplicate ACK\n";
        queueAck(seqNum);
    }
    else {
        // Out of order (seqNum > expectedSeqNum)
        // Go-Back-N: discard and ACK last in-order received
        EV << "Client: Out-of-order packet, discarding\n";
        if (expectedSeqNum > 0) {
            queueAck(expectedSeqNum - 1);
        }
    }
}

void RdtClient::trySendFromBuffer()
{
    while (!sendBuffer.empty() && isChannelIdle()) {
        RdtPacket *pkt = sendBuffer.front();
        sendBuffer.pop_front();

        EV << "Client: Sending " << pkt->getName();
        if (pkt->isAck()) {
            EV << " ackNum=" << pkt->getAckNum();
        }
        EV << "\n";

        send(pkt, "out");
    }

    // Schedule next send if buffer not empty
    if (!sendBuffer.empty()) {
        cGate *outGate = gate("out");
        simtime_t finishTime = outGate->getTransmissionChannel()->getTransmissionFinishTime();
        if (finishTime > simTime() && !sendEvent->isScheduled()) {
            scheduleAt(finishTime, sendEvent);
        }
    }
}

bool RdtClient::isChannelIdle()
{
    cGate *outGate = gate("out");
    simtime_t finishTime = outGate->getTransmissionChannel()->getTransmissionFinishTime();
    return finishTime <= simTime();
}

void RdtClient::finish()
{
    if (requestTimer->isScheduled()) {
        cancelEvent(requestTimer);
    }
    cancelAndDelete(sendEvent);
    cancelAndDelete(requestTimer);

    while (!sendBuffer.empty()) {
        delete sendBuffer.front();
        sendBuffer.pop_front();
    }

    EV << "Client: Finished. Bytes received=" << totalBytesReceived << "\n";
}
