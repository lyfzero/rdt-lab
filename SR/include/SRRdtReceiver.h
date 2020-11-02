//
// Created by lyf on 2020/11/2.
//

#ifndef SR_SRRDTRECEIVER_H
#define SR_SRRDTRECEIVER_H

#include "RdtReceiver.h"
#include <unordered_map>
class SRRdtReceiver :public RdtReceiver
{
private:
    int expectSequenceNumberRcvd;	// 期待收到的下一个报文序号
    Packet lastAckPkt;				// 上次发送的确认报文
    unordered_map<int, Packet> recvMap;   // 缓冲区（滑动窗口）

public:
    SRRdtReceiver();
    virtual ~SRRdtReceiver();

public:

    void receive(const Packet &packet);	//接收报文，将被NetworkService调用
};

#endif //SR_SRRDTRECEIVER_H
