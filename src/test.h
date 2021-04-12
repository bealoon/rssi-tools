#include "define.h"

#include <pthread.h>
#include <fcntl.h>      // define O_WRONLY and O_RDONLY  
#include <sstream>      // for stringstream 
#include <fstream>
#include <string>

using namespace std;

class CTest
{
private:
    /* data */
    static void *threadworker(void *arg);    
public:
    CTest(/* args */);
    ~CTest();

    void read_test_bin();
private:
    pthread_t           threadid        = 0;
    static std::string                      file_path;
};




