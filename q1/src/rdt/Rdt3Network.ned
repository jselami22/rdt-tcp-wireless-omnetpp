import ned.DatarateChannel;

simple RdtClient
{
    parameters:
        int maxMsgSize @unit(B) = default(1000B);
        double requestTime @unit(s) = default(0s);
        @signal[dataPkRcvd](type=long);
        @signal[ackPkSent](type=long);
        @signal[corruptedPkDrop](type=long);
        @signal[dataDelay](type=simtime_t);
        @statistic[dataPkRcvd](title="Data packets received"; source=dataPkRcvd; record=count;);
        @statistic[ackPkSent](title="ACK packets sent"; source=ackPkSent; record=count;);
        @statistic[corruptedPkDrop](title="Corrupted packets dropped"; source=corruptedPkDrop; record=count;);
        @statistic[dataDelay](title="One-way data delay"; source=dataDelay; record=mean,histogram;);
    gates:
        input in;
        output out;
}

simple RdtServer
{
    parameters:
        int maxMsgSize @unit(B) = default(1000B);
        int windowSize = default(100);
        int fileSizeBytes @unit(B) = default(100000B);
        double timeout @unit(s) = default(0.2s);
        @signal[dataPkSent](type=long);
        @signal[dataPkRetx](type=long);
        @signal[ackPkRcvd](type=long);
        @signal[reqPkRcvd](type=long);
        @signal[corruptedPkDrop](type=long);
        @statistic[dataPkSent](title="Data packets sent"; source=dataPkSent; record=count;);
        @statistic[dataPkRetx](title="Data packets retransmitted"; source=dataPkRetx; record=count;);
        @statistic[ackPkRcvd](title="ACK packets received"; source=ackPkRcvd; record=count;);
        @statistic[reqPkRcvd](title="Request packets received"; source=reqPkRcvd; record=count;);
        @statistic[corruptedPkDrop](title="Corrupted packets dropped"; source=corruptedPkDrop; record=count;);
    gates:
        input in;
        output out;
}

channel LossyLink extends ned.DatarateChannel
{
    parameters:
        datarate = default(10Mbps);
        delay = default(10ms);
        per = default(0.1); // packet error rate
        ber = default(0);   // bit error rate
}

network Rdt3Network
{
    submodules:
        client: RdtClient { @display("p=100,200"); }
        server: RdtServer { @display("p=300,200"); }
    connections:
        client.out --> c2s: LossyLink --> server.in;
        server.out --> s2c: LossyLink --> client.in;
}
