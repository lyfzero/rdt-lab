//
// Created by lyf on 2020/11/2.
//


#include "Global.h"
#include "GBNRdtSender.h"


GBNRdtSender::GBNRdtSender():expectSequenceNumberSend(1),waitingState(false),base(1)
{
}


GBNRdtSender::~GBNRdtSender()
{
}



bool GBNRdtSender::getWaitingState() {
    return waitingState;
}




bool GBNRdtSender::send(const Message &message) {
    // if (this->waitingState) { //发送方处于等待确认状态
    //     return false;
    // }
    if (this->expectSequenceNumberSend >= this->base + GBN_N)    // 超出窗口大小，refuse
        return false;

    Packet pkt;
    pkt.acknum = -1;
    pkt.seqnum = this->expectSequenceNumberSend;
    pkt.checksum = 0;
    memcpy(pkt.payload,message.data,sizeof(message.data));
    pkt.checksum = pUtils->calculateCheckSum(pkt);
	pUtils->printPacket("发送方发送报文", pkt);
    pns->sendToNetworkLayer(RECEIVER,pkt);
    this->pktQueue.push_back(pkt);
    if(this->base == this->expectSequenceNumberSend){
        pns->startTimer(SENDER,Configuration::TIME_OUT,-1);
    }
    this->expectSequenceNumberSend++;																					//进入等待状态
    return true;
}

void GBNRdtSender::receive(const Packet &ackPkt) {
    // 检查校验和是否正确
    int checkSum = pUtils->calculateCheckSum(ackPkt);
    if(checkSum != ackPkt.checksum) {
        pUtils->printPacket("发送方没有正确收到确认，数据校验错误", ackPkt);
        return;
    }
    if(ackPkt.acknum >= this->pktQueue.front().seqnum){
        pUtils->printPacket("发送方正确收到确认", ackPkt);
        // 移动滑动窗口
        std::cout << "滑动窗口移动：" << this->base << "---" << ackPkt.acknum + 1 << std::endl;
        base = ackPkt.acknum + 1;
        int pos = 0;
        for(int i = 0; i < this->pktQueue.size(); i++) {
            if(this->pktQueue[i].seqnum == ackPkt.acknum) {
                pos = i;
                break;
            }
        }
        int j = 0;
        while(j <= pos && j < this->pktQueue.size()) {
            this->pktQueue.pop_front();
            j++;
        }
        // 重置timer
        if(this->base == this->expectSequenceNumberSend) {
            pns->stopTimer(SENDER, -1);
        }else{
            pns->stopTimer(SENDER, -1);
            pns->startTimer(SENDER, Configuration::TIME_OUT, -1);
        }

    }
}

void GBNRdtSender::timeoutHandler(int seqNum) {
    pns->startTimer(SENDER,Configuration::TIME_OUT,-1);
    for(auto iter = this->pktQueue.begin();iter !=this->pktQueue.end();iter++){
        if(iter == this->pktQueue.begin()){
        	pUtils->printPacket("发送方定时器时间到，重发上次发送的报文",*iter);
        }
        pns->sendToNetworkLayer(RECEIVER,*iter);
    }
    return;
}
