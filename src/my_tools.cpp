#include "my_tools.h"

#include <sys/time.h>

/**
 * @description:                查询子字符串位置 
 * @param {char} des_total      被查询字节数组
 * @param {const char} src_sub  查询字符数组
 * @param {int} len_total       被查询字节数组长度
 * @param {int} len_sub         查询字符数组长度
 * @return {*} 字符串首位置
 * @author: bbxie
 */
int16_t CMyTools::find_index(unsigned char des_total[], const unsigned char src_sub[], int len_total, int len_sub)
{
    for (int i = 0; i < len_total - len_sub; i++)
    {
        if (memcmp(&des_total[i], src_sub, len_sub) == 0)
        {
            return i;
        }
    }
    return -1;
}



/**
 * @description: 获取时间戳 秒
 * @param {*}
 * @return {*} 时间戳-秒
 * @author: bbxie
 */
uint64_t CMyTools::get_timestamp_s(void)
{
    struct timeval tv;
    gettimeofday(&tv,NULL);
    return tv.tv_sec;
}

/**
 * @description: 获取时间戳 毫秒
 * @param {*}
 * @return {*} 毫秒-时间戳
 * @author: bbxie
 */
uint64_t CMyTools::get_timestamp_ms(void)
{
    struct timeval tv;
    gettimeofday(&tv,NULL);
    return (tv.tv_sec * 1000 + tv.tv_usec/1000);
}