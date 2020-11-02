//
// Created by lyf on 2020/11/2.
//


#include "Global.h"
#include "SRRdtSender.h"


SRRdtSender::SRRdtSender():expectSequenceNumberSend(1),waitingState(false),base(1)
{
}


SRRdtSender::~SRRdtSender()
{
}



bool SRRdtSender::getWaitingState() {
    return waitingState;
}




bool SRRdtSender::send(const Message &message) {
    // 超出窗口大小，refuse
    if(this->expectSequenceNumberSend >= this->base + SR_N){
        return false;
    }

    Packet pkt;
    pkt.acknum = -1;
    pkt.seqnum = this->expectSequenceNumberSend;
    pkt.checksum = 0;
    memcpy(pkt.payload,message.data,sizeof(message.data));
    pkt.checksum = pUtils->calculateCheckSum(pkt);
	pUtils->printPacket("发送方发送报文", pkt);
    pns->sendToNetworkLayer(RECEIVER,pkt);
    pkt.acknum = ACK_UNRCV;
    this->pktMap[this->expectSequenceNumberSend] = pkt;
    // 关联pkt seqnum 
    pns->startTimer(SENDER,Configuration::TIME_OUT,this->expectSequenceNumberSend);
    this->expectSequenceNumberSend++;
    return true;
}

void SRRdtSender::receive(const Packet &ackPkt) {
    // 检查校验和
    int checkSum = pUtils->calculateCheckSum(ackPkt);
    if(checkSum != ackPkt.checksum){
		pUtils->printPacket("发送方没有正确收到接收方确认报文,数据校验错误", ackPkt);
        return ;
    }
    // 窗口最前分组的确认
    if(ackPkt.acknum == this->base){
		pUtils->printPacket("发送方正确收到确认,窗口第一个分组确认", ackPkt);
        this->base = ackPkt.acknum +1;
        this->pktMap.erase(ackPkt.acknum);
        pns->stopTimer(SENDER,ackPkt.acknum);
        // base移动到从窗口开始到第一个没有被确认的seqnum位置
        for(int i = ackPkt.acknum + 1; i < ackPkt.acknum + this->pktMap.size() + 1; i++){
            if(this->pktMap.count(i) != 0){
                auto iter = this->pktMap.find(i);
                if(iter->second.seqnum == ACK_RCV){
                    this->pktMap.erase(i);
                    base++;
                }
                else{
                    break;
                }
            }
        }
        std::cout << "滑动窗口移动: "<< ackPkt.acknum << "-->" << base << std::endl;
    } else if(ackPkt.acknum > this->base && ackPkt.acknum < this->base + SR_N){
        if(this->pktMap.count(ackPkt.acknum) == 0){
            return ;
        }
        auto iter = this->pktMap.find(ackPkt.acknum);
        if(iter->second.seqnum == ACK_UNRCV){
            iter->second.seqnum = ACK_RCV;
            pns->stopTimer(SENDER, ackPkt.acknum);
        }
    }
    return ;
}

void SRRdtSender::timeoutHandler(int seqNum) {
    if(this->pktMap.count(seqNum) == 0){
        return ;
    }
    auto iter = this->pktMap.find(seqNum);
    pns->startTimer(SENDER,Configuration::TIME_OUT, seqNum);  
    pns->sendToNetworkLayer(RECEIVER, iter->second);
    pUtils->printPacket("发送超时包:", iter->second);
    return;
}
