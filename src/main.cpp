/*
 * @Author: xiebibing
 * @since: 2019-09-28 09:36:01
 * @lastTime: 2021-04-13 10:34:40
 * @LastAuthor: Do not edit
 * @message: rssi sensor demo
 */

#include <iostream>
#include <cstring>
#include <unistd.h>
#include <getopt.h>

#include "_system.h"
#include "buffer.h"

using namespace std;

int main(int argc, char *argv[])
{
    int ch;
    const char *format = "h:";
    printf("Usage: %s [-h target IP Address]\n", argv[0]);
    while ((ch = getopt(argc, argv, format)) != -1)
    {
        switch(ch){
            case 'h':
                if (strlen(optarg) > 4)
                {
                    CBuffer::m_RemoteIP = optarg; 
                }
                break;
            case '?':
                printf("unknown params [%c]\n", ch);
                exit(-1);
                break;
        }
    }
    
    // 1. 初始化、启动系统
    CSystem::init(); // 构成系统的各成员变量进行初始化
    CSystem::run();  // 主程序开始执行

    // 2. 等待线程结束
    CSystem::wait(); // 等待系统结束

    // 3.退出系统
    CSystem::destory();
    cout << "*** exit application ***" << endl;

    return 0;
}
