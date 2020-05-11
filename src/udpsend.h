
#pragma once

#include <stdbool.h>
#include <stdint.h>
#include <sys/types.h>
#include <fstream>
#include <iomanip>
#include <netinet/in.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include <semaphore.h>

#include "define.h"

using namespace std;

class udpsend
{
public:
  static int init(void);
  static int senddata(char *lpdata, int size);
  static void destory(void);

  static int clifd;

private:
  static socklen_t socklen;
  static struct sockaddr_in servaddr;
  static struct sockaddr_in cliaddr;
};
