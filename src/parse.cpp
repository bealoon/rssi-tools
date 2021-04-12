#include "parse.h"
#include "my_tools.h"

#include <iostream>
#include <sys/time.h>

using namespace std;

SwapBuffer CParse::m_SwapBuffer = {0};
bool CParse::magic_ok;
uint16_t CParse::m_ChainNumLast = 0;
uint16_t CParse::m_PacketNumLast = 0;


bool CParse::cut_streams(ONE_PACKET_DATA &packet, char *bytes, int len)
{
    if (len + m_SwapBuffer.len > SWAP_BUFFER_MAX_SIZE)
    {
        perror("error length 3\n");
        m_SwapBuffer.len = 0;
        return false;
    }
    memcpy(m_SwapBuffer.buffer + m_SwapBuffer.len, bytes, len);
    m_SwapBuffer.len += len;

    // 查询头部
    uint16_t pos = CMyTools::find_index((unsigned char *)&m_SwapBuffer.buffer, magic_word, m_SwapBuffer.len, 4);
    if (pos >= 0)
    {
        if (pos > 0)
        {
            memcpy(m_SwapBuffer.buffer, (char *)&m_SwapBuffer.buffer[pos], m_SwapBuffer.len - pos);
            m_SwapBuffer.len -= pos;
        }

        if (m_SwapBuffer.len < 6)
        {
            return false;
        }
        memcpy(&m_SwapBuffer.total_length, m_SwapBuffer.buffer + 4, 2);

        if (m_SwapBuffer.len < m_SwapBuffer.total_length + 6)
        {
            cout << "less length 4" << endl;
            return false;
        }
        magic_ok = true;
    }
    //
    if (magic_ok)
    {
        int packet_size = m_SwapBuffer.total_length + 6;
        // cout << "magic OK!!! buffer_size=" << m_SwapBuffer.len << ", packet_size=" << packet_size << endl;
#if PARSE_AUTO_VERSION == 1
        parse_packet_auto(packet, m_SwapBuffer.buffer);
#else 
        parse_packet_sub64(packet, m_SwapBuffer.buffer);
#endif // 0
        memcpy(m_SwapBuffer.buffer, m_SwapBuffer.buffer + packet_size, m_SwapBuffer.len - packet_size);
        m_SwapBuffer.len -= packet_size;
        // cout << " package_length=" << packet.package_length << endl;
        // cout << " version=" << int(packet.version) << endl;
        // cout << " ofdm_type=" << int(packet.ofdm_type) << endl;
        // cout << " rssi=" << (int)(packet.rssi) << endl;
        // cout << " snr=" << int(packet.snr) << endl;
        // cout << " timestamp=" << packet.timestamp << endl;
        // printf(" mac="MACSTR6"\n", mac2str(packet.mac));
        // printf(" %d %d\n", packet.frame_mode, packet.channel_idx);
        // printf(" %d %d\n", packet.tx, packet.rx);
        // printf(" %d %d %d\n", packet.chain_idx, packet.packet_idx, packet.resever);
        // printf(" %d %d %d\n", packet.hbuffer_index.chain_idx, packet.hbuffer_index.chain_last, packet.hbuffer_index.packet_cnt);
        magic_ok = false;
        return true;
    }
    return false;
}

#if PARSE_AUTO_VERSION == 1
bool CParse::parse_packet_auto(ONE_PACKET_DATA &des_packet, char in_bytes[])
{
    memcpy(&des_packet.magic, magic_word, 4);
    int idx = 4;
    memcpy(&des_packet.package_length, in_bytes + idx, 2);
    idx += 2;
    des_packet.version = in_bytes[idx + 3];
    idx += 4;
    des_packet.ofdm_type = in_bytes[idx + 3];
    idx += 4;

    memcpy(&des_packet.timestamp, (char *)&in_bytes[idx + 3], 8);
    idx += 11;
    des_packet.rssi = in_bytes[idx + 3];
    idx += 4;
    des_packet.snr = in_bytes[idx + 3];
    idx += 4;
    des_packet.bw = in_bytes[idx + 3];
    idx += 4;
    des_packet.channel_idx = in_bytes[idx + 3];
    idx += 4;
    memcpy(&des_packet.mac, in_bytes + idx + 3, 6);
    // memcpy(&des_packet.mac, (char *)&in_bytes[idx + 3], 6);
    idx += 9;
    memcpy(&des_packet.extra, (char *)&in_bytes[idx + 3], 4);
    idx += 7;
    unsigned short bytes_subcarrier = 0;
    memcpy(&bytes_subcarrier, in_bytes + idx + 1, 2);
    if (in_bytes[idx] == 8 && bytes_subcarrier < 64)
    {
        return false;
    }
    des_packet.subcarrier = bytes_subcarrier;
    memcpy(&des_packet.csi_IQs, in_bytes + idx + 3, bytes_subcarrier);
    idx += bytes_subcarrier + 3;
    memcpy(&des_packet.csi_IQs[int(bytes_subcarrier/2)], in_bytes + idx + 3, bytes_subcarrier);
    idx += bytes_subcarrier + 3;
    memcpy(&des_packet.tx, (char *)&in_bytes[idx + 3], 2);
    idx += 5;
    memcpy(&des_packet.rx, (char *)&in_bytes[idx + 3], 2);
    idx += 5;
    des_packet.frame_mode = in_bytes[idx + 3];
    idx += 4;
    HBuffer_Index hbuffer_index;
    memcpy(&hbuffer_index, (char *)&in_bytes[idx + 3], 4);
    des_packet.is_chain_last = hbuffer_index.chain_last;
    des_packet.chain_idx = hbuffer_index.chain_idx;
    des_packet.packet_idx = hbuffer_index.packet_cnt;
    idx += 7;
    printf("finish! %d %d idx=%d bytes_subcarrier=%d\n", des_packet.chain_idx, des_packet.packet_idx, idx, bytes_subcarrier);
    return true;
}
#else

bool CParse::parse_packet_sub64(char in_bytes[], ONE_PACKET_DATA &des_packet)
{
    memcpy(&des_packet.magic, magic_word, 4);
    int idx = 4;
    memcpy(&des_packet.package_length, (char *)&in_bytes[idx], 2);
    idx += 2;
    des_packet.version = in_bytes[idx + 3];
    idx += 4;
    des_packet.ofdm_type = in_bytes[idx + 3];
    idx += 4;
    memcpy(&des_packet.timestamp, (char *)&in_bytes[idx + 3], 8);
    idx += 11;
    des_packet.rssi = in_bytes[idx + 3];
    idx += 4;
    des_packet.snr = in_bytes[idx + 3];
    idx += 4;
    des_packet.bw = in_bytes[idx + 3];
    idx += 4;
    des_packet.channel_idx = in_bytes[idx + 3];
    idx += 4;
    memcpy(&des_packet.mac, (char *)&in_bytes[idx + 3], 6);
    idx += 9;
    memcpy(&des_packet.extra, (char *)&in_bytes[idx + 3], 4);
    idx += 7;
    memcpy(&des_packet.csi_is, (char *)&in_bytes[idx + 3], 128);
    idx += 131;
    memcpy(&des_packet.csi_qs, (char *)&in_bytes[idx + 3], 128);
    idx += 131;
    memcpy(&des_packet.tx, (char *)&in_bytes[idx + 3], 2);
    idx += 5;
    memcpy(&des_packet.rx, (char *)&in_bytes[idx + 3], 2);
    idx += 5;
    des_packet.frame_mode = in_bytes[idx + 3];
    idx += 4;
    HBuffer_Index hbuffer_index;
    memcpy(&hbuffer_index, (char *)&in_bytes[idx + 3], 4);
    des_packet.is_chain_last = hbuffer_index.chain_last;
    des_packet.chain_idx = hbuffer_index.chain_idx;
    des_packet.packet_idx = hbuffer_index.packet_cnt;
    idx += 7;
    // printf("finish! %d %d idx=%d\n", des_packet.chain_idx, des_packet.packet_idx, idx);
    return true;
}
#endif // 0

bool CParse::parse_packet_assist(char in_bytes[], packet_assist_t &des_packet)
{
    des_packet.csi_rssi = in_bytes[28];
    des_packet.csi_rssi = in_bytes[32];
    des_packet.csi_phase_incr = 0;

    uint16_t chain_idx = 0;
    uint16_t packet_idx = 0;
    memcpy(&chain_idx, (char *)&in_bytes[335], 2);
    memcpy(&packet_idx, (char *)&in_bytes[337], 2);
    if (chain_idx == m_ChainNumLast)
    {
        return false;
    }

    if (packet_idx == m_PacketNumLast)
    {
        return false;
    }
    return true;
}