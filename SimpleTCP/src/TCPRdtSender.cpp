//
// Created by lyf on 2020/11/2.
//


#include "Global.h"
#include "TCPRdtSender.h"


TCPRdtSender::TCPRdtSender():nextSeqNum(1),waitingState(false),base(1)
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
    if(PkgVec.size() >= TCP_WINDOW_SIZE) {
        return false;
    }
    Packet* pkt = new Packet();
    pkt->acknum = -1;
    pkt->seqnum = nextSeqNum;
    pkt->checksum = 0;
    memcpy(pkt->payload, message.data, sizeof(message.data));
    pkt->checksum = pUtils->calculateCheckSum(*pkt);
    PkgVec.push_back(pkt);

    pUtils->printPacket("发送方发送报文", *pkt);
    pns->sendToNetworkLayer(RECEIVER, *pkt);
    if(base == nextSeqNum) {
        pns->startTimer(SENDER, Configuration::TIME_OUT, 0);
    }
    nextSeqNum++;
    nextSeqNum %= MAX_SEQ;
    waitingState = PkgVec.size() >= TCP_WINDOW_SIZE;
    pkt = nullptr;
    return true;
}

void TCPRdtSender::receive(const Packet &ackPkt) {
    int checkSum = pUtils->calculateCheckSum(ackPkt);
    // 校验和不对，返回
    if(checkSum != ackPkt.checksum){
        pUtils->printPacket("xxxx没有正确收到发送方的报文,数据校验错误", ackPkt);
        return ;
    }
    // 如果没有损坏
    if (((nextSeqNum > base) && (ackPkt.acknum >= base) && (ackPkt.acknum < nextSeqNum)) || ((nextSeqNum < base) && ((ackPkt.acknum < nextSeqNum) || (ackPkt.acknum >= base)))) {
        // 只有当收到的ack是在当前窗口的时候才进行处理, 否则直接丢弃
        // 并将waitingState置为false
        waitingState = false;
        // 先暂存base的值,用于停止计时器
        int temp = base;
        // 更新base的值

        cout << "TCP 发送方收到ack = " << ackPkt.acknum << "窗口发生变化: 由 " << endl;
        printWindow();
        cout << "变为: " << endl;

        base = ackPkt.acknum + 1;
        base %= MAX_SEQ;
        int index = 0;
        if (ackPkt.acknum >= temp) {
            index = ackPkt.acknum - temp;
        } else {
            index = MAX_SEQ - temp + ackPkt.acknum;
        }
        // 假设此时是base = 2 ^ k - 1，然后收到的ack = 0, 那么, 结果应该要删除2个
        for (int i = 0; i <= index; i++) {
            // 遍历每一个packet, 销毁对应的对象
            delete PkgVec.at(i);
        }

        // 移除所有已经接收到的对象指针, 其中base - temp表示收到的个数
        PkgVec.erase(PkgVec.begin(), PkgVec.begin() + index + 1);
        if (base == nextSeqNum) {
            pns->stopTimer(SENDER, 0);
        } else {
            pns->stopTimer(SENDER, 0);
            pns->startTimer(SENDER, Configuration::TIME_OUT, 0);
        }
        if (ackCnt.find(ackPkt.acknum) != ackCnt.end()) {
            // 如果之前就有的话，说明是上一轮留下来的，那么就重置为1
            ackCnt[ackPkt.acknum] = 1;
        } else {
            // 如果没有的话置为1
            ackCnt.insert({ackPkt.acknum, 1});
        }
        printWindow();
    }
    else {
        // 如果不在当前的窗口当中
//            cout << "收到的ack = " << ackPkt.acknum << endl;
//            assert(ackCount.find(ackPkt.acknum) != ackCount.end());
        ackCnt[ackPkt.acknum]++;
        if (ackCnt[ackPkt.acknum] == 4 || ackPkt.acknum == -1) {
            // 如果达到了4个重复的ack，执行快速重传，或者是第一个数据包出错所发送的ack = -1
            Packet * packet = PkgVec.at(0);
            pUtils->printPacket("收到3个重复ack，执行快速重传", *packet);
            pns->sendToNetworkLayer(RECEIVER, *packet);
            ackCnt[ackPkt.acknum] = 0;
        }
    }
}

void TCPRdtSender::timeoutHandler(int seqNum) {
    pns->stopTimer(SENDER, 0);
    pns->startTimer(SENDER, Configuration::TIME_OUT, 0);
    // 只对最开始的一个包进行重传
    Packet * packet = PkgVec.at(0);
    pUtils->printPacket("发送方定时器时间到，重发上次发送的报文", *packet);
    pns->sendToNetworkLayer(RECEIVER, *packet);
    if (ackCnt.find(seqNum) != ackCnt.end()) {
        ackCnt[seqNum] = 0;
    }
}

void TCPRdtSender::printWindow() {
    if (base < nextSeqNum) {
        cout << "[ ";
        for (int i = base; i < nextSeqNum;i++) {
            cout << i << ", ";
        }
        cout << "]" << endl;
    } else if (base > nextSeqNum) {
        cout << "[ ";
        for (int i = base; i < MAX_SEQ; i++) {
            cout << i << ", ";
        }
        for (int i = 0; i < nextSeqNum; i++) {
            cout << i << ", ";
        }
        cout << "]" << endl;
    } else {
        cout << "[ ]" << endl;
    }
}