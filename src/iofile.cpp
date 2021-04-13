#include "define.h"
#include "iofile.h"
#include "buffer.h"
#include "parse.h"
#include "my_tools.h"

#include <pthread.h>
#include <fcntl.h>      // define O_WRONLY and O_RDONLY  
#include <sstream>      // for stringstream 
#include <fstream>


using namespace std;

//  实际应用
string                       CIoFile::file_path      = "/proc/net/wlan/csi_data_0";

uint64_t                     CIoFile::last_tstamp    = 0;
uint16_t                     CIoFile::packet_cnt     = 0;

bool                         CIoFile::is_working     = true;


//构析函数
CIoFile::CIoFile(/* args */)
{

}
//构析函数 释放
CIoFile::~CIoFile()
{
    is_working = false;
    threadid = 0;
}

bool CIoFile::read_file()
{
    pthread_create(&threadid, NULL, threadworker, NULL);
    pthread_join(threadid, NULL);
}

//读取文件线程
void *CIoFile::threadworker(void *arg)
{
    char   read_buffer[512];//数据缓冲
    int    read_buffer_size = 0;
    while (is_working)
    {
        //读取io文件流
        ifstream fin(file_path, std::ios::binary);
        //判断如果失败情况
        if (fin.fail())
        {
            perror("read prop file failed!!!");
            fin.clear();
            fin.close();
            sleep(1);
            continue;
        }

        while (fin.read(read_buffer, PACKET_LENGTH))
        {
            read_buffer_size = fin.gcount(); //看刚才读了多少字节
            // 未读取到文件 则
            if (read_buffer_size == 0)
            {
                usleep(5);
                continue;
            }
            packet_cnt ++;
            uint64_t now_tstamp = CMyTools::get_timestamp_s();
            if (last_tstamp == 0)
            {
                last_tstamp = now_tstamp;
            }
            if (now_tstamp - last_tstamp >= 10)
            {
                printf(" 10s 's number of packet[%d] is: %d\n", read_buffer_size, packet_cnt);
                packet_cnt = 0;
                last_tstamp = now_tstamp;
            }
            //集中处理操作
            ONE_PACKET_DATA packet;
            if(CParse::cut_streams(packet, read_buffer, read_buffer_size)){
                // 攒包 解析 返回成功后
                CBuffer::add_row(packet);
            }
            //等待5ms
            usleep(2);
        }
        //关闭文件
        fin.close();
    }
}

// //读取文件线程
// void *CIoFile::threadworker(void *arg)
// {
//     char   read_buffer[512];//数据缓冲
//     int    read_buffer_size = 0;
//     while (is_working)
//     {
//         //读取io文件流
//         ifstream fin(file_path, std::ios::binary);
//         //判断如果失败情况
//         if (fin.fail())
//         {
//             perror("read prop file failed!!!");
//             fin.clear();
//             fin.close();
//             sleep(1);
//             continue;
//         }

//         fin.read(read_buffer, PACKET_LENGTH);
//         read_buffer_size = fin.gcount(); //看刚才读了多少字节
//         // cout << "read_buffer_size= " << read_buffer_size << endl; 

            // //获取文件的大小
            // fin.seekg(0, fin.end);
            // int length = fin.tellg();
            // fin.seekg(0, fin.beg);
            // printf("length= %d\n", length);

//         //关闭文件
//         fin.close();

//         // 未读取到文件 则
//         if (read_buffer_size == 0)
//         {
//             usleep(5);
//             continue;
//         }
        
//         packet_cnt ++;
//         uint64_t now_tstamp = CMyTools::get_timestamp_s();
//         if (last_tstamp == 0)
//         {
//             last_tstamp = now_tstamp;
//         }
//         if (now_tstamp - last_tstamp >= 10)
//         {
//             printf(" 10s 's number of packet[%d] is: %d\n", read_buffer_size, packet_cnt);
//             packet_cnt = 0;
//             last_tstamp = now_tstamp;
//         }
//         //集中处理操作
//         ONE_PACKET_DATA packet;
//         if(CParse::cut_streams(packet, read_buffer, read_buffer_size)){
//             // 攒包 解析 返回成功后
//             CBuffer::add_row(packet);
//         }
//         //等待5ms
//         usleep(2);
//     }
// }