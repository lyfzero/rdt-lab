//
// Created by lyf on 2020/11/2.
//

#ifndef SIMPLETCP_RDTRECEIVER_H
#define SIMPLETCP_RDTRECEIVER_H

#include "DataStructure.h"
//定义RdtReceiver抽象类，规定了必须实现的一个接口方法
//具体的子类比如StopWaitRdtReceiver、GBNRdtReceiver必须给出这一个方法的具体实现
//只考虑单向传输，即接收方只接收数据
struct  RdtReceiver
{
    virtual void receive(const Packet &packet) = 0;		//接收报文，将被NetworkService调用
    virtual ~RdtReceiver() = 0;
};

#endif //SIMPLETCP_RDTRECEIVER_H
