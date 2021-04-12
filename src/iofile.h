#pragma once

#include "define.h"

#include <deque>
#include <string>

//文件错位偏移 位数
#define IO_POSITION_OFFSET 1

class CIoFile
{

public:
    CIoFile(/* args */);
    ~CIoFile();

    bool read_file();
private:
    static void *threadworker(void *arg);    
    
private:
    /* data */
    static std::string                      file_path;
    static uint64_t                         last_tstamp;
    static uint16_t                         packet_cnt;
    static bool                             is_working;

    pthread_t                               threadid        = 0;
    
};
