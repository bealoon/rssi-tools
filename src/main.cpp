/*
 * @Author: xiebibing
 * @since: 2019-09-28 09:36:01
 * @lastTime: 2019-09-28 09:36:01
 * @LastAuthor: Do not edit
 * @message: rssi sensor demo
 */

#include <iostream>
#include <cstring>

#include "systemprocess.h"

using namespace std;

int main(int argc, char const *argv[])
{
    /* code */
    // 1. 初始化、启动系统
    CSystemProcess::init(); // 构成系统的各成员变量进行初始化
    CSystemProcess::run();  // 主程序开始执行

    // 2. 等待线程结束
    CSystemProcess::wait(); // 等待系统结束

    // 3.退出系统
    CSystemProcess::destory();
    cout << "*** exit application ***" << endl;

    return 0;
}
