//
// Created by lyf on 2020/11/2.
//


#include "Global.h"
#include "SRRdtSender.h"


SRRdtSender::SRRdtSender():nextSeqNum(1),waitingState(false),base(1)
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
    if(PktVec.size() >= SR_WINDOW_SIZE) {
        return false;
    }

    Packet *pkt = new Packet();
    pkt->acknum = -1;
    pkt->seqnum = this->nextSeqNum;
    pkt->checksum = 0;
    memcpy(pkt->payload,message.data,sizeof(message.data));
    pkt->checksum = pUtils->calculateCheckSum(*pkt);
    PktVec.push_back(pkt);
    PktState.push_back(UNKWON);

	pUtils->printPacket("发送方发送报文", *pkt);
    pns->sendToNetworkLayer(RECEIVER, *pkt);
    pns->startTimer(SENDER,Configuration::TIME_OUT, pkt->seqnum);
    this->nextSeqNum++;
    nextSeqNum %= MAX_SEQ;
    waitingState = PktVec.size() >= SR_WINDOW_SIZE;
    pkt = nullptr;
    return true;
}

void SRRdtSender::receive(const Packet &ackPkt) {
    // 检查校验和
    int checkSum = pUtils->calculateCheckSum(ackPkt);
    if(checkSum != ackPkt.checksum){
		pUtils->printPacket("发送方没有正确收到接收方确认报文,数据校验错误", ackPkt);
        return ;
    }
    if (((nextSeqNum > base) && (ackPkt.acknum >= base) && (ackPkt.acknum < nextSeqNum)) || ((nextSeqNum < base) && ((ackPkt.acknum < nextSeqNum) || (ackPkt.acknum >= base)))) {
        // 落在当前窗口
        pns->stopTimer(SENDER, ackPkt.acknum);
        int index = 0;
        if (ackPkt.acknum >= base) {
            // 正常情况
            index = ackPkt.acknum - base;
        } else {
            // 在边界上
            index = MAX_SEQ - base + ackPkt.acknum;
            // 假设 base = 2 ^ k -1，并且收到ack = 1，那么应该是第3个包, 即2 ^ k -1, 0, 1
        }
        // 更改对应的包的状态
        if (ackPkt.acknum == base) {
            cout << "SR 发送方收到ack = " << ackPkt.acknum << "窗口发生变化: 由 " << endl;
            printWindow();
            cout << "变为: " << endl;
        }
        PktState[index] = RECEIVED;
        if (ackPkt.acknum == base) {
            while(PktState.size() > 0 && PktState.at(0) == RECEIVER) {
                // 不断去除已经标记为收到的包
                PktState.erase(PktState.begin());
                PktVec.erase(PktVec.begin());
                base++;
            }
            // 移动窗口开始位置
            base %= MAX_SEQ;
            printWindow();
        }
    }
    waitingState = PktVec.size() >= SR_WINDOW_SIZE;
}

void SRRdtSender::timeoutHandler(int seqNum) {
    // 需要通过seqNum找到对应的序号
    int index = 0;
    if (seqNum >= base) {
        // 如果是正常情况超时
        index = seqNum - base;
    } else {
        // 如果是在边界上
        index = MAX_SEQ - base + seqNum;
        // 假设此时 base = 2 ^ k - 1, seqNum = 1, 那么就是第三个包超时了
    }
    // 找到对应的包并发送，并且重启计时器
    // 理论上找到的包和序号应该对应的上
    pns->sendToNetworkLayer(RECEIVER, *PktVec.at(index));
    pns->startTimer(SENDER, Configuration::TIME_OUT,seqNum);

}

void SRRdtSender::printWindow() {
    if (base < nextSeqNum) {
        cout << "[ ";
        for (int i = base; i < nextSeqNum;i++) {
            if (PktState.at(i - base) == RECEIVED) {
                cout << "-" << i << "-, ";
            } else {
                cout << i << ", ";
            }
        }
        cout << "]" << endl;
    } else if (base > nextSeqNum) {
        cout << "[ ";
        for (int i = base; i < MAX_SEQ; i++) {
            if (PktState.at(i - base) == RECEIVED) {
                cout << "-" << i << "-, ";
            } else {
                cout << i << ", ";
            }
        }
        for (int i = 0; i < nextSeqNum; i++) {
            if (PktState.at(i + MAX_SEQ - base) == RECEIVED) {
                cout << "-" << i << "-, ";
            } else {
                cout << i << ", ";
            }
        }
        cout << "]" << endl;
    } else {
        cout << "[ ]" << endl;
    }

}