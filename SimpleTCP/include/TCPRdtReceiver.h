//
// Created by lyf on 2020/11/2.
//

#ifndef SIMPLETCP_TCPRDTRECEIVER_H
#define SIMPLETCP_TCPRDTRECEIVER_H

#include "RdtReceiver.h"
class TCPRdtReceiver :public RdtReceiver
{
private:
    unsigned int expectSequenceNumberRcvd;	// 期待收到的下一个报文序号
    Packet lastAckPkt;				//上次发送的确认报文

public:
    TCPRdtReceiver();
    virtual ~TCPRdtReceiver();

public:

    void receive(const Packet &packet);	//接收报文，将被NetworkService调用
};

#endif //SIMPLETCP_TCPRDTRECEIVER_H
