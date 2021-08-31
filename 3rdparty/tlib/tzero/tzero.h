/**
 * @brief 
 * 
 * @file tzero.h
 */
#pragma once
#include "tzero.conf.h"

#include <tlib.h>

#ifdef __cplusplus
extern "C"
{
#endif
	/**
	 * @brief 获取时间戳
	 * 
	 * @return 
	 */
	tf tlib_getTime();
	ti64 tlib_getTime_ms();
	ti64 tlib_getTime_us();
	ti64 tlib_getTime_ns();

	/**
	 * @brief 得到watch的耗时, s
	 * 精度: 100ns
	 * 
	 * @param watch 
	 * @return 
	 */
	tf tlib_watch(ti64 *watch);

	/**
	 * @brief 跨平台，得到文件的basename
	 * 
	 * @param path 
	 * @return 
	 */
	tc *tlib_basename(const tc path[]);

	/**
	 * @brief 跨平台, system
	 * 
	 * @param cmd 
	 * @return 
	 */
	ti tlib_system(const tc cmd[]);

	/**
	 * @brief 求str的hash
	 * 
	 * @param str 
	 * @return 
	 */
	tu32 tlib_hash_str(const tc str[]);

	/**
	 * @brief char hex 相互转换
	 * 
	 * @return 
	 */
	ti tlib_atox(tc c);
	tc tlib_xtoa(ti x);

	/**
	 * @brief byte hex 互相转换
	 * 
	 * @return 
	 */
	ti tlib_byteToHex(const tb byte[], const ti len, tc hex[]);
	ti tlib_hexToByte(const tc hex[], const ti len, tb byte[]);

#ifdef __cplusplus
}
#endif
