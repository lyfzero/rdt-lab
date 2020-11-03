//
// Created by lyf on 2020/11/2.
//


#include "Global.h"
#include "GBNRdtSender.h"


GBNRdtSender::GBNRdtSender():nextSeqNum(1),waitingState(false),base(1)
{
}


GBNRdtSender::~GBNRdtSender()
{
}



bool GBNRdtSender::getWaitingState() {
    return waitingState;
}




bool GBNRdtSender::send(const Message &message) {
    if (pktVec.size() >= GBN_WINDOW_SIZE)    // 超出窗口大小，refuse
        return false;

    Packet *pkt = new Packet();
    pkt->acknum = -1;
    pkt->seqnum = this->nextSeqNum;
    pkt->checksum = 0;
    memcpy(pkt->payload, message.data, sizeof(message.data));
    pkt->checksum = pUtils->calculateCheckSum(*pkt);

	pUtils->printPacket("发送方发送报文", *pkt);
    pns->sendToNetworkLayer(RECEIVER, *pkt);
    this->pktVec.push_back(pkt);
    if(this->base == this->nextSeqNum){
        pns->startTimer(SENDER,Configuration::TIME_OUT, pkt->seqnum);
    }
    this->nextSeqNum++;																		//进入等待状态
    this->nextSeqNum %= MAX_SEQ;
    waitingState = this->pktVec.size() >= GBN_WINDOW_SIZE;
    pkt = nullptr;
    return true;
}

void GBNRdtSender::receive(const Packet &ackPkt) {
    // 检查校验和是否正确
    int checkSum = pUtils->calculateCheckSum(ackPkt);
    if(checkSum != ackPkt.checksum) {
        pUtils->printPacket("发送方没有正确收到确认，数据校验错误", ackPkt);
        return;
    }
    if (((this->nextSeqNum > this->base) && (ackPkt.acknum >= base) && (ackPkt.acknum < this->nextSeqNum)) || ((this->nextSeqNum < this->base) && ((ackPkt.acknum < this->nextSeqNum) || (ackPkt.acknum >= this->base)))) {
        waitingState = false;
        pUtils->printPacket("发送方正确收到确认", ackPkt);
        int tempBase = base;
        printWindow();
        // 移动滑动窗口
        std::cout << "滑动窗口移动：" << this->base << "---" << ackPkt.acknum + 1 << std::endl;
        base = ackPkt.acknum + 1;
        base %= MAX_SEQ;
        printWindow();

        int index = 0;
        if(ackPkt.acknum >= tempBase) {
            index = ackPkt.acknum - tempBase;
        } else {
            index = MAX_SEQ - tempBase + ackPkt.acknum;
        }

        for(int i = 0; i <= index; i++) {
            delete this->pktVec.at(i);
        }
        this->pktVec.erase(this->pktVec.begin(), this->pktVec.begin() + index + 1);
        // 重置timer
        if(this->base == this->nextSeqNum) {
            pns->stopTimer(SENDER, tempBase);
        }else{
            pns->stopTimer(SENDER, tempBase);
            pns->startTimer(SENDER, Configuration::TIME_OUT, base);
        }

    }
}

void GBNRdtSender::timeoutHandler(int seqNum) {
    pns->stopTimer(SENDER, seqNum);
    pns->startTimer(SENDER,Configuration::TIME_OUT,seqNum);
    int limit = 0;
    if (nextSeqNum > base) {
        // 如果没有越过分界点
        limit = nextSeqNum - base;
    } else if (nextSeqNum < base) {
        // 如果越过了分界点
        limit = MAX_SEQ - base + nextSeqNum;
        // 假设此时base = 2 ^ k - 1, nextsequm = 1, 那么应该重发2个包
    }
    for (int i = 0; i < limit; i++){
        // 遍历从base到nextseqnum-1的所有报文,进行重传
        Packet * packet = pktVec.at(i);
        pUtils->printPacket("发送方定时器时间到，重发上次发送的报文", *packet);
        pns->sendToNetworkLayer(RECEIVER, *packet);
    }
}

void GBNRdtSender::printWindow() {
    if(base < nextSeqNum) {
        std::cout << "[ ";
        for(int i = base; i < nextSeqNum; i++) {
            std::cout << i << ", ";
        }
        std::cout << "]" << endl;
    } else if (base > nextSeqNum) {
        cout << "[ ";
        for (int i = base; i < MAX_SEQ; i++)
            std::cout << i << ", ";
        for(int i=0; i < nextSeqNum; i++)
            std::cout << i << ", ";
        std::cout << "]" << endl;
    } else {
        std::cout << "[ ]" << endl;
    }
}