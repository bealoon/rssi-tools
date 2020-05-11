#pragma once

#include <stdint.h>

#define MAX_UDP_PACKETS_NUM 		80		//攒包发送的包数
#define RFBUF_MAX_TP 				2     	// 接收缓冲数量

typedef int BOOL;
#define TRUE    1
#define FALSE   0

//

//本地调试端
#define UDP_SERVR_IP "127.0.0.1"
#define UDP_SERVR_PORT 10000

typedef struct _One_Packet_Data
{
	uint64_t num;
    uint32_t rate;
    uint32_t rssi_a;
    uint32_t rssi_b;

	_One_Packet_Data()
	{
		num = 0;
		rate = 0;
		rssi_a = 0;
		rssi_b = 0;
	}
}ONE_PACKET_DATA, *PONE_PACKET_DATA;   

