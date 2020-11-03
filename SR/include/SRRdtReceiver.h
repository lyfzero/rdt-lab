//
// Created by lyf on 2020/11/2.
//

#ifndef SR_SRRDTRECEIVER_H
#define SR_SRRDTRECEIVER_H

#include "RdtReceiver.h"
#include <vector>
class SRRdtReceiver :public RdtReceiver
{
private:
    Packet lastAckPkt;				// 上次发送的确认报文
    vector<Packet *> PktbufferVec;
    vector<int> PktbufferState;
    int base;
    enum {RECEIVED, UNKNOW};

    void printWindow();
    void push_empty_packet();
    void copy_packet(const Packet *source, Packet *dest);

public:
    SRRdtReceiver();
    virtual ~SRRdtReceiver();

public:

    void receive(const Packet &packet);	//接收报文，将被NetworkService调用
};

#endif //SR_SRRDTRECEIVER_H
