#include <iostream>
#include <stdio.h>
#include <semaphore.h>
#include <arpa/inet.h>
#include <linux/socket.h>
#include <unistd.h>
#include <stdint.h>
#include <sys/types.h>
#include <sys/time.h>
#include <signal.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>

#include "udpsend.h"

using namespace std;

socklen_t udpsend::socklen;
int udpsend::clifd;
struct sockaddr_in udpsend::servaddr;
struct sockaddr_in udpsend::cliaddr;

int udpsend::init(void)
{

    /* 建立udp socket */
    clifd = socket(AF_INET, SOCK_DGRAM, 0);
    if (clifd < 0)
    {
        perror("socket");
    }

    /* 设置address */
    socklen = sizeof(servaddr);
    memset(&servaddr, 0, socklen);
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr(UDP_SERVR_IP);
    servaddr.sin_port = htons(UDP_SERVR_PORT);

    return 0;
}

int udpsend::senddata(char *lpdata, int size)
{
    int send_num = sendto(clifd, lpdata, size, 0, (struct sockaddr *)&servaddr, socklen);
    if (send_num == -1){
        cout << " *** SEND ERROR " << endl;
        return -1;
    }
    return send_num;
}

void udpsend::destory(void)
{
    close(clifd);
}
