#pragma once

#include "define.h"

#define PACKET_LENGTH       340 

const unsigned char          magic_word[4] = { 0xaa, 0xbb, 0xcc, 0xdd };
const unsigned char          magic_tail[4] = { 0x00, 0x00, 0x00, 0x00 };

typedef struct _SwapBuffer_t
{
    char                         buffer[SWAP_BUFFER_MAX_SIZE];
    unsigned short               len;
    unsigned short               total_length;  
} SwapBuffer;


class CParse
{
public:                       
    //切割数据包
    static bool cut_streams(ONE_PACKET_DATA &des_packet, char *bytes, int len);                         //切割数据包

    static bool parse_packet_sub64(char in_bytes[], ONE_PACKET_DATA &des_packet); //解析数据包
    static bool parse_packet_auto(ONE_PACKET_DATA &des_packet, char in_bytes[]); //解析数据包

    static bool parse_packet_assist(char in_bytes[], packet_assist_t &des_packet); //解析数据包
private:
    
    static SwapBuffer                   m_SwapBuffer;
    static bool                         magic_ok;
    static uint16_t                     m_ChainNumLast;
    static uint16_t                     m_PacketNumLast;
};
