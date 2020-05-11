#include "streambuffer.h"
#include "udpsend.h"

#include <iostream>

using namespace std;

sem_t                           CStreamBuffer::sendSem;   
std::deque<ONE_PACKET_DATA>     CStreamBuffer::mParsePacketDeq;
ONE_PACKET_DATA                 CStreamBuffer::mSendResult[MAX_UDP_PACKETS_NUM];

pthread_mutex_t                 CStreamBuffer::mPacketDataDeqMutex;


CStreamBuffer::CStreamBuffer(/* args */)
{
    //信号量初始化
    sem_init(&sendSem,0,0);
}

CStreamBuffer::~CStreamBuffer()
{
    //构西函数 清空
    udpsend::destory();
    sem_destroy(&sendSem);
    mParsePacketDeq.clear();
}

bool CStreamBuffer::init(void)
{
    udpsend::init();
}

void CStreamBuffer::addRow(deque<ONE_PACKET_DATA> packets)
{
    pthread_mutex_lock(&mPacketDataDeqMutex);
    mParsePacketDeq.insert(mParsePacketDeq.end(),packets.begin(),packets.end());
    pthread_mutex_unlock(&mPacketDataDeqMutex);
    if (mParsePacketDeq.size() >= MAX_UDP_PACKETS_NUM)
    {
        sem_post(&sendSem);
    }
    // cout << " buffer deque packets size:"  << mParsePacketDeq.size() <<  endl;
}

bool CStreamBuffer::sendPackets(void){
    pthread_mutex_lock(&mPacketDataDeqMutex);
    for (size_t i = 0; i < MAX_UDP_PACKETS_NUM; i++)
    {
        mSendResult[i].num = mParsePacketDeq.front().num;
        mSendResult[i].rate = mParsePacketDeq.front().rate;
        mSendResult[i].rssi_a = mParsePacketDeq.front().rssi_a;
        mSendResult[i].rssi_b = mParsePacketDeq.front().rssi_b;
        mParsePacketDeq.pop_front();
    }
    if (mParsePacketDeq.size() > 50000)
    {
        mParsePacketDeq.clear();
    }
    pthread_mutex_unlock(&mPacketDataDeqMutex);
    
    // udp发送
    int ret = udpsend::senddata((char *)&mSendResult,sizeof(ONE_PACKET_DATA)*MAX_UDP_PACKETS_NUM);
    if(ret != -1){
        cout << " SEND SUCCESS!!! "<< mParsePacketDeq.size() << endl;
    }
    // cout << " send finish deque packets size:"  << mParsePacketDeq.size() <<  endl;
    return true;
}
