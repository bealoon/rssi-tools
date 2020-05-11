#include "systemprocess.h"
#include "streambuffer.h"
#include "iostreamfile.h"

#include <iostream>

using namespace std;

//epoll 
CIOStreamFile   mIOStreamFile;

pthread_t       CSystemProcess::threadid; 


void CSystemProcess::init(void){
    cout << "*** system::init()";
    CStreamBuffer::init();
    cout << " OK ***" << endl;
}

void CSystemProcess::run(){
    cout << "*** system::run()" << endl;
    pthread_create(&threadid, NULL, threadworker, NULL);
    // 读取io文件
    mIOStreamFile.ReadIoFile();
}


void CSystemProcess::wait(void){
    cout << "*** system::wait()" << endl;
    pthread_join(threadid, NULL);

}

void *CSystemProcess::threadworker(void *arg)
{
    cout << "*** system::threadworker run ***" << endl;
    while (true)
    {
        //等待信号通知
        sem_wait(&CStreamBuffer::sendSem);
 
        bool ret = CStreamBuffer::sendPackets();  
    }
    
}

void CSystemProcess::destory(void){
    
}
