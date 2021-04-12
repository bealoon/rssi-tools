#include "buffer.h"
#include "udpsend.h"

#include <iostream>

using namespace std;

sem_t                           CBuffer::sendSem;               //< 线程通信-信号量
std::string                     CBuffer::m_RemoteIP = UDP_SERVR_IP;

pthread_mutex_t                 CBuffer::deqPacketDataMutex;    //< 线程通信-互斥锁
std::deque<ONE_PACKET_DATA>     CBuffer::deqPacketData;         //< 处理数据队列


CBuffer::CBuffer(/* args */)
{
    sem_init(&sendSem,0,0);     //信号量初始化
    
}

CBuffer::~CBuffer()
{
    //构西函数 清空
    udpsend::destory();
    sem_destroy(&sendSem);
    deqPacketData.clear();
}

bool CBuffer::init(void)
{
    udpsend::init();
}

void CBuffer::add_rows(deque<ONE_PACKET_DATA> packets)
{
    pthread_mutex_lock(&deqPacketDataMutex);
    deqPacketData.insert(deqPacketData.end(),packets.begin(),packets.end());
    pthread_mutex_unlock(&deqPacketDataMutex);
    if (deqPacketData.size() >= MAX_UDP_PACKETS_NUM)
    {
        sem_post(&sendSem);
    }
    cout << " buffer deque packets size:"  << deqPacketData.size() <<  endl;
}

void CBuffer::add_row(ONE_PACKET_DATA packet)
{
    pthread_mutex_lock(&deqPacketDataMutex);
    deqPacketData.push_back(packet);
    pthread_mutex_unlock(&deqPacketDataMutex);
    if (deqPacketData.size() >= MAX_UDP_PACKETS_NUM)
    {
        sem_post(&sendSem);
    }
    // cout << " buffer deque packets size:"  << deqPacketData.size() <<  endl;
}

// 从队列中提取前面的数据 UDP发送
bool CBuffer::send_packet(void){
    ONE_PACKET_DATA packet;
    pthread_mutex_lock(&deqPacketDataMutex);
    packet = deqPacketData.front();
    deqPacketData.pop_front();
    // 如果队列数据有阻塞 则清空
    if (deqPacketData.size() > 5)
    {
        deqPacketData.clear();
    }
    pthread_mutex_unlock(&deqPacketDataMutex);
    
    // udp发送
    int ret = udpsend::senddata((char *)&packet, sizeof(ONE_PACKET_DATA) - 2048 + 2 * packet.subcarrier);
    
    if(ret == -1){
        cout << " UDP Send Failed!!! " << endl;
    }
    // for (size_t i = 0; i < 5; i++)
    // {
    //     /* code */
    //     printf("%d ", packet.csi_IQs[2 * packet.subcarrier + i -3]);
    // }
    // printf("<<<\n");
    return true;
}