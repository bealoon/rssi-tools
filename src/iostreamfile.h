#pragma once

#include "define.h"

#include <deque>
#include <string>

//文件错位偏移 位数
#define IO_POSITION_OFFSET 1

class CIOStreamFile
{
private:
    /* data */
    static std::string                      ioFilePath;
    static std::deque<ONE_PACKET_DATA>      deqValidRssiPackets;
    static uint64_t                         last_max_num;
    static uint32_t                         last_pos;

    FILE            *ioFp;
    pthread_t       threadid;  
    


private:
    static void *threadworker(void *arg);    
public:
    CIOStreamFile(/* args */);
    ~CIOStreamFile();

    bool ReadIoFile();
    
};
