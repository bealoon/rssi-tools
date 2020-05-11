#pragma once

#include <pthread.h>

class CSystemProcess
{
private:
    /* data */
    static pthread_t threadid;  
public:

    static void init(void); // 系统初始化
    static void wait(void);         // 等待结束
    static void run(void);  // 运行
    static void destory(void);      // 结束时关闭   

    static void *threadworker(void *arg);                      
};

