//
// Created by lyf on 2020/11/2.
//


#include "Global.h"
#include "SRRdtReceiver.h"


SRRdtReceiver::SRRdtReceiver():base(1)
{
    for (int i = 0; i < SR_WINDOW_SIZE; i++) {
        push_empty_packet();
    }
    lastAckPkt.acknum = -1; //初始状态下，上次发送的确认包的确认序号为-1，使得当第一个接受的数据包出错时该确认报文的确认号为-1
    lastAckPkt.checksum = 0;
    lastAckPkt.seqnum = -1;	//忽略该字段
    for(int i = 0; i < Configuration::PAYLOAD_SIZE;i++){
        lastAckPkt.payload[i] = '.';
    }
    lastAckPkt.checksum = pUtils->calculateCheckSum(lastAckPkt);
}


SRRdtReceiver::~SRRdtReceiver()
{
}

void SRRdtReceiver::receive(const Packet &packet) {
    //检查校验和是否正确
    int checkSum = pUtils->calculateCheckSum(packet);
    if (checkSum != packet.checksum) {
        pUtils->printPacket("接收方没有正确收到发送方的报文,数据校验错误", packet);
        return;    
    }
    // 序号在[base, base + N - 1]的情况
    bool in_case1 = false;
    bool in_case2 = false;
    int index = 0;
    if (base + SR_WINDOW_SIZE <= MAX_SEQ) {
        //此时没有跨越边界
        if (packet.seqnum >= base && packet.seqnum <= base + SR_WINDOW_SIZE - 1) {
            in_case1 = true;
            index = packet.seqnum - base;
        }
    } else {
        // 跨越了边界，通过两个序号一起加上N来跳出边界
        if ((packet.seqnum + SR_WINDOW_SIZE) % MAX_SEQ >= (base + SR_WINDOW_SIZE) % MAX_SEQ &&
            (packet.seqnum + SR_WINDOW_SIZE) % MAX_SEQ <= (base + 2 * SR_WINDOW_SIZE - 1) % MAX_SEQ) {
            in_case1 = true;
            index = (packet.seqnum + SR_WINDOW_SIZE) % MAX_SEQ - (base + SR_WINDOW_SIZE) % MAX_SEQ;
        }
    }

    // 序号在[base - N, base - 1]的情况
    if (base >= SR_WINDOW_SIZE) {
        // 此时base距离序号0至少还有N个空间
        if (packet.seqnum >= base - SR_WINDOW_SIZE && packet.seqnum <= base - 1) {
            in_case2 = true;
        }
    } else {
        // base距离0不足N个空间，两者一起加上N来跨越边界
        if ((packet.seqnum + SR_WINDOW_SIZE) % MAX_SEQ >= base &&
            (packet.seqnum + SR_WINDOW_SIZE) % MAX_SEQ <= base + SR_WINDOW_SIZE - 1) {
            in_case2 = true;
        }
    }
    // 开始处理
    if (in_case1) {
        if (packet.seqnum == base) {
            cout << "接收方收到数据包 seq = " << packet.seqnum << "窗口发生变化: 由 " << endl;
            printWindow();
            cout << "变为: " << endl;
        }
        lastAckPkt.acknum = packet.seqnum;
        lastAckPkt.checksum = 0;
        lastAckPkt.checksum = pUtils->calculateCheckSum(lastAckPkt);
        pUtils->printPacket("接收方发送确认报文", lastAckPkt);
        pns->sendToNetworkLayer(SENDER, lastAckPkt);    //调用模拟网络环境的sendToNetworkLayer，通过网络层发送确认报文到对方
        if (PktbufferState.at(index) == UNKNOW) {
            PktbufferState[index] = RECEIVED;
            Packet *temp = PktbufferVec.at(index);
            // 复制到对应的位置
            copy_packet(&packet, temp);
            temp = nullptr;
        }
    }
    if (in_case2) {
        // 如果是第二种情况也要正常发送ack
        lastAckPkt.acknum = packet.seqnum;
        lastAckPkt.checksum = 0;
        lastAckPkt.checksum = pUtils->calculateCheckSum(lastAckPkt);
        pUtils->printPacket("接收方发送确认报文", lastAckPkt);
        pns->sendToNetworkLayer(SENDER, lastAckPkt);    //调用模拟网络环境的sendToNetworkLayer，通过网络层发送确认报文到对方
    }
    if (packet.seqnum == base) {
        // 如果等于基序号
        while (PktbufferState.at(0) == RECEIVED) {
            Message msg;
            memcpy(msg.data, PktbufferVec.at(0)->payload, sizeof(PktbufferVec.at(0)->payload));
            pns->delivertoAppLayer(RECEIVER, msg);
            PktbufferState.erase(PktbufferState.begin());
            PktbufferVec.erase(PktbufferVec.begin());
            push_empty_packet();
            base++;
            base %= MAX_SEQ;
        }
        printWindow();
    }
}

void SRRdtReceiver::printWindow() {
    cout << "接收方window";
    if (PktbufferState.size() == 0) {
        cout << "[ ]" << endl;
        return;
    }
    cout << "[ ";
    for (int i = 0; i < PktbufferState.size(); i++) {
        if (PktbufferState.at(i) == RECEIVED) {
            cout << "-" << (i + base) % MAX_SEQ << "-, ";
        } else {
            cout << (i + base) % MAX_SEQ << ", ";
        }
    }
    cout << "]" << endl;
}

void SRRdtReceiver::push_empty_packet() {
    // 填充新的数据到缓冲区，每个字段的值并不重要
    Packet *packet = new Packet();
    PktbufferVec.push_back(packet);
    PktbufferState.push_back(UNKNOW);
    packet = nullptr;
}

void SRRdtReceiver::copy_packet(const Packet *source, Packet *dest) {
    dest->checksum = source->checksum;
    dest->acknum = source->acknum;
    dest->seqnum = source->acknum;
    for (int i = 0; i < Configuration::PAYLOAD_SIZE; i++) {
        dest->payload[i] = source->payload[i];
    }
}