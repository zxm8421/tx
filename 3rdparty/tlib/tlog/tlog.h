/**
 * @file tlog.h
 * 
 * @brief 
 * 
 */
#pragma once

#include <tlib.h>

#define TLOG_BUF_SIZE 4096
#define TLOG_CONSOLE_ENABLE 1
#define TLOG_FILE_ENABLE 1
#define TLOG_FILE_NUM 10
#define TLOG_FILE_SIZE (10 * 1024 * 1024)
#define TLOG_FILE_DIR "log"
#define TLOG_FILE_PREFIX "tx"

// 日志等级
#define TLOG_D 0 // DEBUG	调试信息
#define TLOG_I 1 // INFO	正常信息
#define TLOG_W 2 // WARNING	可处理的问题，比如丢包，畸形包等
#define TLOG_E 3 // ERROR	正常情况下不应该出现，需要立刻修复甚至停止运行
#define TLOG_T 4 // TEST	单元测试使用

#define TLOG_ALL 0	// 允许全部日志
#define TLOG_OFF 16 // 关闭全部日志

// 默认文件日志等级
#define TLOG_LOCAL_FILTER TLOG_ALL

/**
 * @brief 日志打印函数, 语法同printf
 * 日志会同时打印到console和file
 * 会在末尾添加'\\n'
 * 
 * @param level 
 * @param format 
 * @param ... 
 * @return 
 */
#define tlog(level, format, ...) tlog_rawprint(__FILE__, __LINE__, __func__, TLOG_LOCAL_FILTER, level, format, ##__VA_ARGS__)

/**
 * @brief hex打印函数
 * 
 * @param level 
 * @param info 
 * @param ptr 
 * @param len 
 * @return 
 */
#define tlog_hexdump(level, info, ptr, len) tlog_rawhexdump(__FILE__, __LINE__, __func__, TLOG_LOCAL_FILTER, level, info, ptr, len)

#ifdef __cplusplus
extern "C"
{
#endif
	/**
	 * @brief tlog 初始化
	 * 
	 * @return 
	 */
	ti tlog_init();

	/**
	 * @brief 设置全局日志过滤级别
	 * 
	 * @param filter 
	 * @return 
	 */
	ti tlog_set_global_filter(ti filter);

	/**
 	 * @brief 请勿直接使用，请使用tlog
 	 * 
 	 * @param file 
 	 * @param line 
 	 * @param func 
 	 * @param filter 
 	 * @param level 
 	 * @param format 
 	 * @param ... 
 	 * @return 
 	 */
	ti tlog_rawprint(const tc *file, const ti line, const tc *func, const ti filter, const ti level, const tc *format, ...);

	/**
	 * @brief 请勿直接使用，请使用tlog_hexdump
	 * 
	 * @param file 
	 * @param line 
	 * @param func 
	 * @param filter 
	 * @param level 
	 * @param info 
	 * @param ptr 
	 * @param len 
	 * @return 
	 */
	ti tlog_rawhexdump(const tc *file, const ti line, const tc *func, const ti filter, const ti level, const tc *info, const void *ptr, const ti len);

	/**
	 * @brief tlog自用, 打印函数
	 * 用于tlog没起来的场合，以及tlog调试自己 \
	 * 会在末尾添加'\\n'
	 * 
	 * 
	 * @param format 
	 * @param ... 
	 * @return 
	 */
	ti tlog_debug(const tc *format, ...);

#ifdef __cplusplus
}
#endif
