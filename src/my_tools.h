#pragma one

#include "define.h"

class CMyTools
{
public:
    
    static int16_t find_index(unsigned char des_total[], const unsigned char src_sub[], int len_total, int len_sub);

    
    // 获取本地时间戳 秒
    static uint64_t     get_timestamp_s(void);
    // 获取本地时间戳 毫秒
    static uint64_t     get_timestamp_ms(void);
};

