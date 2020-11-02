//
// Created by lyf on 2020/11/2.
//


#include "Global.h"
#include "SRRdtReceiver.h"


SRRdtReceiver::SRRdtReceiver():expectSequenceNumberRcvd(1)
{
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
    // 窗口最前分组的确认
    if(packet.seqnum == this->expectSequenceNumberRcvd){
		pUtils->printPacket("sender-recv:接收方正确收到", packet);
        this->expectSequenceNumberRcvd = packet.seqnum + 1;
		Message msg;
		memcpy(msg.data, packet.payload, sizeof(packet.payload));
		pns->delivertoAppLayer(RECEIVER, msg);
        // base移动到从窗口开始到第一个没有被确认的seqnum位置
		for(int i = packet.seqnum + 1; i < packet.seqnum + SR_N; i++){
			if(this->recvMap.count(i) != 0){
				auto iter = this->recvMap.find(i);
				Message msg;
				memcpy(msg.data, iter->second.payload, sizeof(iter->second.payload));
				pns->delivertoAppLayer(RECEIVER, msg);
				this->recvMap.erase(i);
				this->expectSequenceNumberRcvd++;
			} else {
				break;
			}
		}
		this->lastAckPkt.acknum = packet.seqnum;
		this->lastAckPkt.checksum = pUtils->calculateCheckSum(this->lastAckPkt);
		pUtils->printPacket("接收方发送确认报文", this->lastAckPkt);
		pns->sendToNetworkLayer(SENDER, this->lastAckPkt);
    }else if(packet.seqnum > this->expectSequenceNumberRcvd && packet.seqnum < this->expectSequenceNumberRcvd + SR_N){
		if(this->recvMap.count(packet.seqnum) == 0){
			this->recvMap[packet.seqnum] = packet ;
		}	
		this->lastAckPkt.acknum = packet.seqnum;
		this->lastAckPkt.checksum = pUtils->calculateCheckSum(this->lastAckPkt);
		pUtils->printPacket("接收方发送确认报文", this->lastAckPkt);
		pns->sendToNetworkLayer(SENDER, this->lastAckPkt);
    }else if(packet.seqnum < this->expectSequenceNumberRcvd && packet.seqnum >= this->expectSequenceNumberRcvd - SR_N){
		this->lastAckPkt.acknum = packet.seqnum;
		this->lastAckPkt.checksum = pUtils->calculateCheckSum(this->lastAckPkt);
		pUtils->printPacket("接收方发送确认报文", this->lastAckPkt);
		pns->sendToNetworkLayer(SENDER, this->lastAckPkt);
	}
    return ;
}