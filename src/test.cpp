#include "define.h"
#include "test.h"
#include "parse.h"
#include "buffer.h"

#include <iostream>

using namespace std;


// string CTest::file_path      = "/mnt/d/linux/02-0329-mobile/二进制文件/may_error2.bin";
string CTest::file_path      = "/mnt/d/linux/02-0329-mobile/二进制文件/wuren.bin";

CTest::CTest(/* args */)
{
}

CTest::~CTest()
{
}

void CTest::read_test_bin()
{
    pthread_create(&threadid, NULL, threadworker, NULL);
    pthread_join(threadid, NULL);
}


//读取文件线程
void *CTest::threadworker(void *arg)
{
    char buffer[340];
    //读取io文件流
    ifstream fin(file_path.c_str(), ios::binary);
    //判断如果失败情况
    if (fin.fail())
    {
        perror("read prop file failed!!!");
        fin.clear();
        fin.close();
        exit(-1);
    }
    ONE_PACKET_DATA packet;
    while(fin.read(buffer, sizeof(buffer))) { //一直读到文件结束
        int readedBytes = fin.gcount(); //看刚才读了多少字节

        cout << "readedBytes= " << readedBytes << endl; 
        ONE_PACKET_DATA packet;

        //集中处理操作
        if(CParse::cut_streams(packet, buffer, readedBytes)){
            // 攒包 解析 返回成功后
            CBuffer::add_row(packet);
        }
        // sleep(1);
        usleep(500);
    }
    //关闭文件
    fin.close();
}