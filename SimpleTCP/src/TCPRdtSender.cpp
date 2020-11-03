//
// Created by lyf on 2020/11/2.
//


#include "Global.h"
#include "TCPRdtSender.h"


TCPRdtSender::TCPRdtSender():expectSequenceNumberSend(1),waitingState(false),base(1),timer_status(false),accumulate_sum(0)
{
}


TCPRdtSender::~TCPRdtSender()
{
}

bool TCPRdtSender::getWaitingState() {
    return waitingState;
}

bool TCPRdtSender::send(const Message &message) {
    // 超出窗口大小，refuse
    if(after(this->expectSequenceNumberSend,this->base + TCP_N)){
        return false;
    }
    Packet pkt;
    pkt.acknum = -1;
    pkt.seqnum = this->expectSequenceNumberSend;
    pkt.checksum = 0;
    memcpy(pkt.payload, message.data, sizeof(message.data));
    pkt.checksum = pUtils->calculateCheckSum(pkt);
    pUtils->printPacket("发送方发送报文", pkt);
    pns->sendToNetworkLayer(RECEIVER, pkt);
    this->pktQueue.push_back(pkt);
    if(this->timer_status == false){
        pns->startTimer(SENDER, Configuration::TIME_OUT,-1);
        this->timer_status = true;
    }
    this->expectSequenceNumberSend += sizeof(message.data) ;
    return true;
}

void TCPRdtSender::receive(const Packet &ackPkt) {
    int checkSum = pUtils->calculateCheckSum(ackPkt);
    // 校验和不对，返回
    if(checkSum != ackPkt.checksum){
        pUtils->printPacket("xxxx没有正确收到发送方的报文,数据校验错误", ackPkt);
        return ;
    }
    if(after(ackPkt.acknum, this->base)){
        std::cout << "滑动窗口移动: "<< this->base << "--->"<< ackPkt.acknum << std::endl;
        this->base = ackPkt.acknum;
        if(this->timer_status == true) {
            pns->stopTimer(SENDER,-1);
        }
        //累计确认
        while (this->pktQueue.size() !=0 && before(this->pktQueue.front().seqnum, this->base))
        {
            this->pktQueue.pop_front();
        }
        //还有未被确认的包
        if(this->pktQueue.size() != 0){
            pns->startTimer(SENDER, Configuration::TIME_OUT,-1);
            this->timer_status = true;
        }
    }
    if(ackPkt.acknum == this->base){
        this->accumulate_sum++;
        // 三次冗余ACK 重传
        if(this->accumulate_sum == 3){
            auto pkt = this->pktQueue.front();
            pUtils->printPacket("超时重传报文", pkt);
            pns->sendToNetworkLayer(RECEIVER, pkt);
            this->accumulate_sum = 0;
        }
    }
}

void TCPRdtSender::timeoutHandler(int seqNum) {
    if(this->pktQueue.size() != 0 ){
        auto pkt = this->pktQueue.front();
        pns->sendToNetworkLayer(RECEIVER, pkt);
        pns->startTimer(SENDER, Configuration::TIME_OUT,-1);
        this->timer_status = true;
    }
    return;

}
