#pragma once 

#include "define.h"

#include <semaphore.h>
#include <string>
#include <deque>


class CBuffer
{

// 方法
public:
    CBuffer(/* args */);
    ~CBuffer();

    static bool init(void);
    static void add_rows(std::deque<ONE_PACKET_DATA> packets);
    static void add_row(ONE_PACKET_DATA packet);
    static bool send_packet(void);


// 属性
private:
    // 处理队列buffer
    static std::deque<ONE_PACKET_DATA>  deqPacketData;
    static pthread_mutex_t              deqPacketDataMutex;

public:
    static sem_t                        sendSem;
    static std::string                  m_RemoteIP;
};
