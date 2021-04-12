#include "_system.h"
#include "buffer.h"
#include "iofile.h"

#include "test.h"

#include <iostream>

using namespace std;

//文件读取线程 
CIoFile         mIoFile;
pthread_t       CSystem::threadid; 


void CSystem::init(void){
    cout << "*** system::init()";
    CBuffer::init();
    cout << " OK ***" << endl;
}

void CSystem::run(){
    cout << "*** system::run()" << endl;
    pthread_create(&threadid, NULL, threadworker, NULL);
    // 读取io文件
    mIoFile.read_file();

    // CTest m_Test;
    // m_Test.read_test_bin();
}


void CSystem::wait(void){
    cout << "*** system::wait()" << endl;
    pthread_join(threadid, NULL);

}

void *CSystem::threadworker(void *arg)
{
    cout << "*** system::threadworker run ***" << endl;
    while (true)
    {
        //等待信号通知
        sem_wait(&CBuffer::sendSem);
 
        CBuffer::send_packet();  
    }
    
}

void CSystem::destory(void)
{
    
}
