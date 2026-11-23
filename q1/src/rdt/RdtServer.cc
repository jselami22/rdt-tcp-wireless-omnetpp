#include <omnetpp.h>
#include <deque>
#include <set>
#include "../rdt/RdtPacket_m.h"

using namespace omnetpp;

class RdtServer : public cSimpleModule
{
  private:
    // Parameters
    int maxMsgSize;
    int windowSize;
    int fileSizeBytes;
    simtime_t timeout;

    // State variables
    int nextSeqNum;          // Next seq to be sent
    int base;                // Oldest unACKed seq
    int totalSegments;       // Total number of segments
    bool transferStarted;
    bool transferComplete;

    // Track sent packets for statistics
    std::set<int> firstTimeSent;  // Packets sent for first time

    // Buffer for packets waiting to be sent
    std::deque<RdtPacket*> sendBuffer;
    cMessage *sendEvent;

    // Timer
    cMessage *timeoutTimer;
    int timeoutCount;

    // Signals
    simsignal_t dataPkSentSignal;
    simsignal_t dataPkRetxSignal;
    simsignal_t ackPkRcvdSignal;
    simsignal_t reqPkRcvdSignal;
    simsignal_t corruptedPkDropSignal;

  protected:
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
    virtual void finish() override;

    void startTransfer();
    void createAndQueuePacket(int seqNum, bool isRetransmission);
    void sendFinPacket();
    void handleAck(RdtPacket *ackPkt);
    void handleTimeout();
    void startTimer();
    void stopTimer();
    void trySendFromBuffer();
    bool isChannelIdle();
};

Define_Module(RdtServer);

void RdtServer::initialize()
{
    maxMsgSize = par("maxMsgSize");
    windowSize = par("windowSize");
    fileSizeBytes = par("fileSizeBytes");
    timeout = par("timeout");

    base = 0;
    nextSeqNum = 0;
    transferStarted = false;
    transferComplete = false;
    timeoutCount = 0;

    // Calculate total segments
    totalSegments = (fileSizeBytes + maxMsgSize - 1) / maxMsgSize;

    timeoutTimer = new cMessage("timeout");
    sendEvent = new cMessage("sendEvent");

    // Register signals
    dataPkSentSignal = registerSignal("dataPkSent");
    dataPkRetxSignal = registerSignal("dataPkRetx");
    ackPkRcvdSignal = registerSignal("ackPkRcvd");
    reqPkRcvdSignal = registerSignal("reqPkRcvd");
    corruptedPkDropSignal = registerSignal("corruptedPkDrop");

    EV << "Server initialized: totalSegments=" << totalSegments
       << " windowSize=" << windowSize << " timeout=" << timeout << "\n";
}

void RdtServer::handleMessage(cMessage *msg)
{
    if (msg == timeoutTimer) {
        handleTimeout();
    }
    else if (msg == sendEvent) {
        trySendFromBuffer();
    }
    else {
        RdtPacket *pkt = check_and_cast<RdtPacket*>(msg);

        // Check for bit errors
        if (pkt->hasBitError()) {
            EV << "Server: Corrupted packet dropped\n";
            emit(corruptedPkDropSignal, 1L);
            delete pkt;
            return;
        }

        // Handle request
        if (pkt->isRequest()) {
            EV << "Server: Request received\n";
            emit(reqPkRcvdSignal, 1L);
            if (!transferStarted) {
                startTransfer();
            }
        }
        // Handle ACK
        else if (pkt->isAck()) {
            handleAck(pkt);
        }

        delete pkt;
    }
}

void RdtServer::startTransfer()
{
    transferStarted = true;
    base = 0;
    nextSeqNum = 0;
    firstTimeSent.clear();
    timeoutCount = 0;

    EV << "Server: Starting transfer\n";

    // Fill initial window
    int toSend = std::min(windowSize, totalSegments);
    for (int i = 0; i < toSend; i++) {
        createAndQueuePacket(nextSeqNum++, false);
    }

    // Start sending
    if (!sendBuffer.empty()) {
        trySendFromBuffer();
        startTimer();
    }
}

void RdtServer::createAndQueuePacket(int seqNum, bool isRetransmission)
{
    if (seqNum >= totalSegments) {
        return;
    }

    RdtPacket *pkt = new RdtPacket("DATA");
    pkt->setIsData(true);
    pkt->setSeqNum(seqNum);

    // Calculate payload
    int remaining = fileSizeBytes - (seqNum * maxMsgSize);
    int payload = std::min(maxMsgSize, remaining);
    pkt->setPayloadBytes(payload);
    pkt->setByteLength(64 + payload);

    // Track statistics
    if (isRetransmission) {
        EV << "Server: Retransmit seqNum=" << seqNum << "\n";
        emit(dataPkRetxSignal, 1L);
    } else {
        if (firstTimeSent.find(seqNum) == firstTimeSent.end()) {
            EV << "Server: New packet seqNum=" << seqNum << "\n";
            emit(dataPkSentSignal, 1L);
            firstTimeSent.insert(seqNum);
        } else {
            // Already sent before, count as retransmission
            EV << "Server: Re-send seqNum=" << seqNum << "\n";
            emit(dataPkRetxSignal, 1L);
        }
    }

    sendBuffer.push_back(pkt);
}

void RdtServer::trySendFromBuffer()
{
    while (!sendBuffer.empty() && isChannelIdle()) {
        RdtPacket *pkt = sendBuffer.front();
        sendBuffer.pop_front();

        EV << "Server: Sending " << pkt->getName() << " seqNum=" << pkt->getSeqNum() << "\n";
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

bool RdtServer::isChannelIdle()
{
    cGate *outGate = gate("out");
    simtime_t finishTime = outGate->getTransmissionChannel()->getTransmissionFinishTime();
    return finishTime <= simTime();
}

void RdtServer::sendFinPacket()
{
    RdtPacket *finPkt = new RdtPacket("FIN");
    finPkt->setIsFin(true);
    finPkt->setByteLength(64);

    EV << "Server: Sending FIN\n";
    sendBuffer.push_back(finPkt);
    trySendFromBuffer();
}

void RdtServer::handleAck(RdtPacket *ackPkt)
{
    int ackNum = ackPkt->getAckNum();

    EV << "Server: ACK received ackNum=" << ackNum
       << " base=" << base << " nextSeqNum=" << nextSeqNum << "\n";

    emit(ackPkRcvdSignal, 1L);

    // Check if this ACK is useful (acks new data)
    if (ackNum < base) {
        EV << "Server: Old ACK ignored\n";
        return;
    }

    // Move base forward (cumulative ACK)
    base = ackNum + 1;
    timeoutCount = 0;  // Reset timeout counter

    EV << "Server: Base advanced to " << base << "\n";

    // Stop timer
    stopTimer();

    // Check if done
    if (base >= totalSegments) {
        if (!transferComplete) {
            transferComplete = true;
            EV << "Server: Transfer complete!\n";
            sendFinPacket();
        }
        return;
    }

    // Send new packets to fill window
    while (nextSeqNum < base + windowSize && nextSeqNum < totalSegments) {
        createAndQueuePacket(nextSeqNum++, false);
    }

    // Try sending queued packets
    trySendFromBuffer();

    // Restart timer if there are outstanding packets
    if (base < nextSeqNum) {
        startTimer();
    }
}

void RdtServer::handleTimeout()
{
    timeoutCount++;

    EV << "Server: TIMEOUT #" << timeoutCount
       << " base=" << base << " nextSeqNum=" << nextSeqNum << "\n";

    if (timeoutCount > 1000) {
        EV << "Server: Too many timeouts, aborting\n";
        endSimulation();
        return;
    }

    // Go-Back-N: retransmit all packets in window [base, nextSeqNum)
    for (int i = base; i < nextSeqNum && i < totalSegments; i++) {
        createAndQueuePacket(i, true);
    }

    // Send queued packets
    trySendFromBuffer();

    // Restart timer
    startTimer();
}

void RdtServer::startTimer()
{
    if (timeoutTimer->isScheduled()) {
        cancelEvent(timeoutTimer);
    }

    // Use exponential backoff
    simtime_t backoff = timeout * (1.0 + std::min(timeoutCount, 10) * 0.5);
    scheduleAt(simTime() + backoff, timeoutTimer);

    EV << "Server: Timer started with timeout=" << backoff << "\n";
}

void RdtServer::stopTimer()
{
    if (timeoutTimer->isScheduled()) {
        cancelEvent(timeoutTimer);
        EV << "Server: Timer stopped\n";
    }
}

void RdtServer::finish()
{
    stopTimer();

    // Clean up
    cancelAndDelete(timeoutTimer);
    cancelAndDelete(sendEvent);

    while (!sendBuffer.empty()) {
        delete sendBuffer.front();
        sendBuffer.pop_front();
    }

    EV << "Server: Finished. base=" << base << " totalSegments=" << totalSegments << "\n";
}
