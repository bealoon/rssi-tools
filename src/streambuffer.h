#pragma once 

#include "define.h"

#include <semaphore.h>
#include <string>
#include <deque>

class CStreamBuffer
{
private:
    static std::deque<ONE_PACKET_DATA> mParsePacketDeq;
    static ONE_PACKET_DATA mSendResult[MAX_UDP_PACKETS_NUM];
    
    static pthread_mutex_t mPacketDataDeqMutex;
public:
    CStreamBuffer(/* args */);
    ~CStreamBuffer();

    static bool init(void);
    static void addRow(std::deque<ONE_PACKET_DATA> packets);
    static bool sendPackets(void);
public:
    static sem_t sendSem;
};
