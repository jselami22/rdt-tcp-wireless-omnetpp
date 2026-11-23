# Reliable Data Transfer & TCP over Wireless (OMNeT++)

A project I did for my computer networks class. It implements the Stop-and-Wait and Go-Back-N protocols in OMNeT++ and compares them over a lossy channel, then studies how antenna power affects TCP performance in a multi-hop wireless network built with the INET Framework.

## Folder Structure

```
CompNet_Project/
├── q1/
│   ├── src/rdt/
│   │   ├── Rdt3Network.ned
│   │   ├── RdtPacket.msg
│   │   ├── RdtClient.cc
│   │   └── RdtServer.cc
│   ├── omnetpp.ini
│   └── results/
│       └── *.sca
│
├── q2/
│   ├── omnetpp.ini
│   └── results/
│       └── *.sca
│
├── docs/
│   └── *.png
├── REPORT.md
└── README.md
```

`RdtPacket_m.cc` and `RdtPacket_m.h` are not included, OMNeT++ generates them from `RdtPacket.msg` when you build the project.

## Implementation

## Q1: Stop-and-Wait & Go-Back-N

### RdtClient.cc

Sends a file request to the server
Receives data packets and checks for corruption using `hasBitError()`
Delivers in-order packets and discards duplicates/out-of-order packets
Sends cumulative ACKs for received packets

### RdtServer.cc

Waits for the client request, then sends the file in segments
Maintains a sliding window of size N (N=1 for Stop-and-Wait)
On timeout, retransmits all unACKed packets in the window (Go-Back-N behavior)
Sends a FIN packet when the transfer is complete

## Q2: TCP Performance in Wireless Networks

### omnetpp.ini

Extends the Wireless13 configuration from the INET Framework wireless tutorial
Replaces the UDP applications with TCP (`TcpBasicClientApp` and `TcpGenericServerApp`)
Configures an antenna power sweep: 5mW, 10mW, 20mW, 50mW
Sets static mobility for the relay nodes
Enables recording of TCP state variables (cwnd, ssthresh, RTT, RTO)

## Results

The `results/` folders contain `.sca` files from representative simulation runs.
q1 contains the runs with PER = 0.0, Window Size = 1 and PER = 0.1, Window Size = 100.
q2 contains the run with Antenna Power = 10mW.

The `.vec` files are not included because of their size (over 50 MB), you can get them by running the simulations again.

## Report

The results, graphs and analysis are in [REPORT.md](REPORT.md).

## How to Run

Requires OMNeT++ 6 and the INET Framework 4.

Q1: import the `q1` folder as an OMNeT++ project, build it, and run `omnetpp.ini`. The `channelPer` and `windowSize` values in the ini file run every combination.

Q2: copy `q2/omnetpp.ini` into INET's `tutorials/wireless` folder and run the `Wireless13_TCP_Static` configuration.