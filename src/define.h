#pragma once

#include <iostream>
#include <unistd.h>  
#include <stdint.h>
#include <stdlib.h>  
#include <string.h>  


#define MAX_UDP_PACKETS_NUM 		1					//攒包发送的包数

typedef int 						BOOL;
#define TRUE    					1
#define FALSE   					0


#define PARSE_AUTO_VERSION 			1 

//本地调试端
// #define UDP_SERVR_IP 				"127.0.0.1"
#define UDP_SERVR_IP 				"192.168.1.155"
#define UDP_SERVR_PORT 				10000


#define CSI_NUM_TX 					2
#define CSI_NUM_RX 					2
#define CSI_SUNCARRIER 				64

// 交换buffer的最大size
#define SWAP_BUFFER_MAX_SIZE 		2048


/**
 * @brief CSI H矩阵结构体 
	按位排布
 */
typedef struct
{
    unsigned chain_idx : 15;
	unsigned chain_last: 1;
    unsigned packet_cnt: 15;
} HBuffer_Index;

#if PARSE_AUTO_VERSION == 1 //PARSE_AUTO_VERSION
// amp auto
typedef struct _One_Packet_Data
{
	uint8_t 		magic[4];			// 魔咒
	uint8_t 		mac[6];				// 源包mac
	uint8_t 		extra[4];			// a bitmap for bring the information of the CSI
	uint8_t 		version;			// 版本号
	uint8_t 		ofdm_type;			// 模式类型 0: CCK 1: OFDM BW20 2: OFDM BW40 3: OFDM BW80
	uint8_t 		bw;					// 带宽
	uint8_t 		channel_idx;		// 主信道idx
	uint8_t			frame_mode;			// 帧模式 0: CCK packet 1: Legacy OFDM packet 2: Mixed-mode HT packet 3: Greenfield HT packet 4: VHT packet 8: HE-SU Packet
	uint8_t			is_chain_last;		// 是否是最后一包
	int8_t 			rssi;				// RSSI
	int8_t 			snr;				// SNR
	uint16_t 		package_length;		// 包长度
	uint16_t		rx;					// 接收天线
	uint16_t		tx;					// 发送天线
	uint16_t		chain_idx;			// 链路索引号
	uint16_t		packet_idx;			// 数据包索引号
	uint64_t 		timestamp;			// 时间戳 Unit is ms
	uint16_t		subcarrier;			// 子载波长度
	uint8_t			reserved[2];		// 预留
	int16_t 		csi_IQs[1024];		// CSI IQ值

}ONE_PACKET_DATA, *PONE_PACKET_DATA;  

#else
// sub64
typedef struct _One_Packet_Data
{
	uint8_t 		magic[4];
	uint8_t 		mac[6];
	uint8_t 		extra[4];
	uint8_t 		version;
	uint8_t 		ofdm_type;
	uint8_t 		bw;
	uint8_t 		channel_idx;
	uint8_t			frame_mode;
	uint8_t			is_chain_last;//预留
	int8_t 			rssi;
	int8_t 			snr;
	uint16_t 		package_length;
	uint16_t		rx;
	uint16_t		tx;
	uint16_t		chain_idx;
	uint16_t		packet_idx;
	uint64_t 		timestamp;
	int16_t 		csi_is[64];
	int16_t 		csi_qs[64];

}ONE_PACKET_DATA, *PONE_PACKET_DATA;  

#endif // PARSE_AUTO_VERSION

// 上位机udp调试用结构体 V2
// typedef struct _pack_host_udp_data {
//     char header[4];          // 电文头
//     unsigned int time_stamp; ///< 时间戳(s)
//     short result;            // 预测结果1-有人 2-无人
//     short saved;             // 保留占位
//     unsigned long cur_prob;  // 缓冲中的最新值
//     unsigned long cur_cp;    //(unsigned long)p_uart_csi.csi_rssi;
//     unsigned long cur_tlen;  //(unsigned long)p_uart_csi.csi_snr;
//     unsigned long cur_ppt;   //(unsigned long)p_uart_csi.csi_phase_incr;
//     short raw_csi_amp[ALL_CSI]; // CSI振幅数据（原始）
//     char tailer[4];             // 电文尾
// }pack_host_udp_data_t;



// tcp输出电文 运行模式电文(release)
typedef struct _packet_release{	
	short						result;						// 预测结果1-有人 2-无人
	unsigned char 				forward;					// 是否转发给生活云
	unsigned char 				cur_switch_status;			// 插座当前开关状态;0关,1开
	short 						alarm_on_threshold;			// 报警启动阈值
	short 						alarm_off_threshold;		// 报警关闭阈值
	unsigned int 				cur_prob; 					// 缓冲中的最新值
	unsigned int 				cur_cp;
	unsigned int 				cur_tlen;
	unsigned int 				cur_ppt;
	unsigned int 				max_cp_val;					//统计时间内最大cp值
	unsigned int				cp_threshold_cnt;			//cp值域统计包数
	unsigned char				cp_dynamic_on_threshold;	//cp动态值域on
	unsigned char 				cp_dynamic_off_threshold;	//cp动态值域off
	unsigned char 				cp_result;					//cp动态阈值有人无人结果
	unsigned char  				t1;
	unsigned short				t2;
	unsigned short				t3;
	unsigned char 				light_state_r;				//红灯状态
	unsigned char 				light_state_g;				//绿灯状态
	unsigned char 				light_state_b;				//蓝灯状态
	unsigned char 				light_state_w;				//白灯状态
	unsigned char 				buz_state;					//蜂鸣器状态
	unsigned char 				sys_err_code; 				//故障码
	char						rssi;
	unsigned char 				saved_3[1];
	float						voltage;
	float 						current;
	float						active_power;
	float						theta;
	float						power;
}packet_release_t;

// tcp输出电文 debug00/01电文 
typedef struct _packet_assist{
	char 			raw_csi[CSI_NUM_RX * CSI_NUM_TX * CSI_SUNCARRIER];			// CSI振幅数据(原始)
	char  			csi_rssi;
	char  			reserved[3];
	unsigned short 	csi_snr;
	short 			csi_phase_incr;
}packet_assist_t;


#define MACSTR6         "%02x:%02x:%02x:%02x:%02x:%02x"
// func mac地址转STR 
#define mac2str(a)      (a)[0], (a)[1], (a)[2], (a)[3], (a)[4], (a)[5]

