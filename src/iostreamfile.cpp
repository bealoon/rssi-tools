#include "iostreamfile.h"
#include "streambuffer.h"
#include "define.h"

#include <iostream>
#include <pthread.h>
#include <fstream>
#include <stdio.h>  
#include <stdlib.h>  
#include <string.h>  
#include <unistd.h>  
#include <fcntl.h>   //define O_WRONLY and O_RDONLY  
#include <sstream>


using namespace std;

//示例文件路径 可更改
//  测试
string                              CIOStreamFile::ioFilePath = "./testRssi.txt";
//  实际应用
// string                              CIOStreamFile::ioFilePath = "/proc/net/rtl8192eu/eth2/rssi_log";

deque<ONE_PACKET_DATA>              CIOStreamFile::deqValidRssiPackets;
uint64_t                            CIOStreamFile::last_max_num = 0;
uint32_t                            CIOStreamFile::last_pos = -1;

//构析函数
CIOStreamFile::CIOStreamFile(/* args */)
{
    
}
//构析函数 释放
CIOStreamFile::~CIOStreamFile()
{

}

bool CIOStreamFile::ReadIoFile()
{
    pthread_create(&threadid, NULL, threadworker, NULL);
    pthread_join(threadid, NULL);
}

//读取文件线程
void *CIOStreamFile::threadworker(void *arg)
{
    while (true)
    {
        //清空有效rssi数组内容
        // deqValidRssiPackets.clear();
        deque<ONE_PACKET_DATA>().swap(deqValidRssiPackets);
        //读取io文件流
        ifstream inFile(ioFilePath);
        //存储行数据 string格式
        string lineStr;
        //判断如果失败情况
        if (inFile.fail())
        {
            perror("Read File Failed!!!");
            exit(-1);
        }
        //存储读取文件的第一行的pos值
        int current_pos = 0;
        //存储读取文件所有行的最大值
        uint64_t current_max_num = 0;

        //定义行标
        int rawIdx = 0;
        //临时存储 所有的行数据
        deque<ONE_PACKET_DATA> *deqTotalPacket = new deque<ONE_PACKET_DATA>();
        //第一行 current_pos
        getline(inFile, lineStr);
        int val = atoi(lineStr.substr(lineStr.find(":")+1, lineStr.size()).c_str());
        if (val == 0)
            current_pos = 128 - IO_POSITION_OFFSET;
        else
            current_pos = val - IO_POSITION_OFFSET;
        // current_pos = val - IO_POSITION_OFFSET;
        //读取所有的rssi行
        while (getline(inFile, lineStr))
        {
            stringstream liness(lineStr);
            string spaceStr;
            //1~128行 rssi 数组
            ONE_PACKET_DATA ppacket;
            int i = 0 ;
            while (std::getline(liness, spaceStr,' '))//每一行数据的解析
            {
                // uint64_t val = atoi(spaceStr.substr(spaceStr.find(":")+1, spaceStr.size()).c_str());
                uint64_t val = strtoull(spaceStr.substr(spaceStr.find(":") + 1, spaceStr.size()).c_str(), NULL, 10);
                // if (val < 0)
                // {
                //     printf("2. Convert 2 Int Failed val: %ld\n",val);
                //     exit(-1);
                // }
                if (i == 0)
                    ppacket.num = val;
                else if ( i == 1)
                    ppacket.rate = val;
                else if ( i == 2)
                    ppacket.rssi_a = val;
                else if ( i == 3)
                    ppacket.rssi_b = val;
                i++;
            }
            deqTotalPacket->push_back(ppacket);
            rawIdx++;
        }
        //关闭文件
        inFile.close();
        //pos位置的num是最大值。pos+1位置的数据是最小的
        current_max_num = (*deqTotalPacket)[current_pos].num;
        printf("deqTotalPacket[current_pos].num=%ld, current_pos=%d, last_max_num=%ld, current_max_num=%ld,deqTotalPacket.size=%ld\n",(*deqTotalPacket)[current_pos].num,current_pos,last_max_num,current_max_num,deqTotalPacket->size());
        if (current_max_num <= last_max_num)
        {
            //do nothing
            printf("do nothing  %d\n",rawIdx);
            //清空临时变量
            delete deqTotalPacket;
            usleep(10);
            continue;
        }
        //不丢包的情况
        if (current_max_num - last_max_num < 128)//
        {
            // printf("normal packets, last_pos=%d, current_pos=%d\n",last_pos,current_pos);
            // 首次读取 上一次的 pos=0
            if (last_pos == -1)
            {
                if (current_pos > last_pos)
                {
                    for (size_t i = 0 ; i <= current_pos; i++)
                    {
                        deqValidRssiPackets.push_back((*deqTotalPacket)[i]);
                    }
                }
                else
                {
                    for (size_t i = 0 ; i <= current_pos + 128; i++)
                    {
                        deqValidRssiPackets.push_back((*deqTotalPacket)[i%128]);
                    }
                }
            }
            else
            {
                if (current_pos > last_pos)
                {
                    for (size_t i = last_pos + 1 ; i <= current_pos; i++)
                    {
                        deqValidRssiPackets.push_back((*deqTotalPacket)[i]);
                    }
                }
                else
                {
                    for (size_t i = last_pos + 1 ; i <= current_pos + 128; i++)
                    {
                        deqValidRssiPackets.push_back((*deqTotalPacket)[i%128]);
                    }
                }
            } 
        }
        else
        {//丢包了 
            printf("lost packets from %ld to %ld\n",last_max_num,(*deqTotalPacket)[current_pos].num);
            for (size_t i = current_pos + 1; i <= current_pos + 128; i++)
            {
                deqValidRssiPackets.push_back((*deqTotalPacket)[i%128]);
            }
        }
        printf("deqValidRssiPackets size= %ld\n",deqValidRssiPackets.size());
        //把当前最大值赋值保存 便于下次使用
        // last_max_num    = current_max_num;
        // last_pos        = current_pos;
        last_max_num = 0;
        last_pos = 1;
        //集中处理操作
        CStreamBuffer::addRow(deqValidRssiPackets);
        //清空临时变量
        delete deqTotalPacket;
        //等待5ms
        usleep(1000);
    }
}