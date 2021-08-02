/**
 * @brief 
 * 
 * @file tlog.h
 */
#pragma once

#if (TLOG != 1)
#define TLOG 1
#endif

/**
 * @brief 使用方法

在每个C文件开头

#define TLOG_TAG "tag"
#define TLOG_LVL TLOG_D
#include <tlog/tlog.h>

 * 
 */

/**
 * @brief 配置项
 * 
 */
#define TLOG_USE_UTC 0
#define TLOG_BUF_SIZE 4096
#define TLOG_CONSOLE_LEVEL TLOG_ALL
#define TLOG_FILE_LEVEL TLOG_ALL
#define TLOG_FILE_NUM 10
#define TLOG_FILE_SIZE (10 * 1024 * 1024)
#define TLOG_FILE_DIR "./log"
#define TLOG_FILE_PREFIX "tx"

/**
 * @brief 日志等级
 * DEBUG	调试信息
 * INFO		正常信息
 * WARNING	可处理的问题，比如丢包，畸形包等
 * ERROR	正常情况下不应该出现的，需要立刻修复甚至停止运行
 * 
 * ALL	允许全部日志
 * OFF	关闭全部日志
 */
#define TLOG_D 0
#define TLOG_I 1
#define TLOG_W 2
#define TLOG_E 3

#define TLOG_ALL 0
#define TLOG_OFF 10

#ifndef TLOG_TAG
#define TLOG_TAG "none"
#endif

#ifndef TLOG_LVL
#define TLOG_LVL TLOG_I
#endif

#include <tjz/ttype.h>

/**
 * @brief 日志打印函数, 语法同printf
 * 日志会同时打印到console和file
 * 会在末尾添加'\n'
 * 
 * @param level 
 * @param format 
 * @param ... 
 * @return 
 */
#define tlog(level, format, ...) tlog_print((const tc *)TLOG_TAG, (ti)TLOG_LVL, (ti)level, (const tc *)__FILE__, (ti)__LINE__, (const tc *)__PRETTY_FUNCTION__, (const tc *)format, ##__VA_ARGS__)

/**
 * @brief hex打印函数
 * 
 * @param level 
 * @param info 
 * @param ptr 
 * @param len 
 * @return 
 */
#define tlog_hexdump(level, info, ptr, len) tlog_print_hexdump((const tc *)TLOG_TAG, (ti)TLOG_LVL, (ti)level, (const tc *)__FILE__, (ti)__LINE__, (const tc *)__PRETTY_FUNCTION__, (const tc *)info, ptr, (ti)len)

#ifdef __cplusplus
extern "C"
{
#endif

	/**
	 * @brief 获取时间戳, ms
	 * 
	 * @return ms
	 */
	ti64 tlog_getTimeMs();

	/**
	 * @brief 获取时间戳, us
	 * 
	 * @return us
	 */
	ti64 tlog_getTimeUs();

	/**
	 * @brief 将时间戳转换为时间日期字符串
	 * 
	 * @param us 
	 * @param use_utc 是否使用UTC时间
	 * @param timeStr eg: "20000101.080000.000000 +0800" "20000101.000000.000000"
	 * @return 转化的字符串长度
	 */
	ti tlog_getTimeStr(const ti64 us, ti use_utc, tc *timeStr);

	/**
	 * @brief 尝试手动滚动日志文件
	 * 日志文件会自动滚动，只有在希望提前滚动日志文件的时候手动调用本函数
	 * 
	 * @param limit B, 限制单个日志文件大小
	 * @return 
	 */
	ti tlog_try_rotate(ti64 limit);

	/**
	 * @brief 设定全局console过滤级别
	 * 
	 * @param level 
	 * @return 
	 */
	ti tlog_set_console_level(ti level);

	/**
	 * @brief 设置全局file过滤级别
	 * 
	 * @param level 
	 * @return 
	 */
	ti tlog_set_file_level(ti level);

	/**
 	* @brief 请勿直接使用
 	* 
 	* @param tag 
 	* @param tag_level 
 	* @param level 
 	* @param file 
 	* @param line 
 	* @param func 
 	* @param format 
 	* @param ... 
 	* @return 
 	*/
	ti tlog_print(const tc *tag, const ti tag_level, const ti level, const tc *file, const ti line, const tc *func, const tc *format, ...);

	/**
	 * @brief 请勿直接使用
	 * 
	 * @param tag 
	 * @param tag_level 
	 * @param level 
	 * @param file 
	 * @param line 
	 * @param func 
	 * @param info 
	 * @param ptr 
	 * @param len 
	 * @return 
	 */
	ti tlog_print_hexdump(const tc *tag, const ti tag_level, const ti level, const tc *file, const ti line, const tc *func, const tc *info, const void *ptr, ti len);

#ifdef __cplusplus
}
#endif