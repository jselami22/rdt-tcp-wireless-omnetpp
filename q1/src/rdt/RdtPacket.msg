packet RdtPacket
{
    int seqNum = -1;   // data segment index (0..), or unused for control
    int ackNum = -1;   // cumulative ACK (largest in-order seq received)
    bool isData = false;    // true => data segment
    bool isAck = false;     // true => ACK
    bool isFin = false;     // true => FIN (file transfer complete)
    bool isRequest = false; // true => file request (from client to server)
    int payloadBytes = 0;   // size of application data carried in this packet
}
