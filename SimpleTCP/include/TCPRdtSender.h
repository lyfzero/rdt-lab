//
// Created by lyf on 2020/11/2.
//

#ifndef SIMPLETCP_TCPRDTSENDER_H
#define SIMPLETCP_TCPRDTSENDER_H

#include "RdtSender.h"
#include <vector>
#include <map>
class TCPRdtSender :public RdtSender
{
private:
    bool waitingState;				// 是否处于等待Ack的状态
    int base;
    int nextSeqNum;
    vector<Packet *> PkgVec;
    map<int, int> ackCnt;

    void printWindow();
public:

    bool getWaitingState();
    bool send(const Message &message);						//发送应用层下来的Message，由NetworkServiceSimulator调用,如果发送方成功地将Message发送到网络层，返回true;如果因为发送方处于等待正确确认状态而拒绝发送Message，则返回false
    void receive(const Packet &ackPkt);						//接受确认Ack，将被NetworkServiceSimulator调用
    void timeoutHandler(int seqNum);					//Timeout handler，将被NetworkServiceSimulator调用

public:
    TCPRdtSender();
    virtual ~TCPRdtSender();
};

#endif //SIMPLETCP_TCPRDTSENDER_H
