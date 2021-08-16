/**
 * @brief 
 * 
 * @file tlog.h
 */
#pragma once

/**
 * @brief 使用方法

在每个c/cpp文件开头

#define TLOG_LOCAL_FILTER TLOG_I
#include <tlog/tlog.h>

 * 
 */

/**
 * @brief 配置项
 * 
 */
#define TLOG_USE_UTC 0
#define TLOG_BUF_SIZE 4096
#ifdef NTEST
#ifdef NDEBUG
#define TLOG_GLOBAL_FILTER TLOG_I
#else
#define TLOG_GLOBAL_FILTER TLOG_ALL
#endif
#else
#define TLOG_GLOBAL_FILTER TLOG_T
#endif

#define TLOG_ECHO_QPS 1
#define TLOG_CONSOLE_ENABLE 1
#define TLOG_FILE_ENABLE 1
#define TLOG_FILE_NUM 10
#define TLOG_FILE_SIZE (10 * 1024 * 1024)
#define TLOG_FILE_DIR "log"
#define TLOG_FILE_PREFIX "tx"

/**
 * @brief 日志等级
 * DEBUG	调试信息
 * INFO		正常信息
 * WARNING	可处理的问题，比如丢包，畸形包等
 * ERROR	正常情况下不应该出现，需要立刻修复甚至停止运行
 * TEST		单元测试使用
 * 
 * ALL	允许全部日志
 * OFF	关闭全部日志
 */
#define TLOG_D 0
#define TLOG_I 1
#define TLOG_W 2
#define TLOG_E 3
#define TLOG_T 4

#define TLOG_ALL 0
#define TLOG_OFF 16

#ifndef TLOG_LOCAL_FILTER
#define TLOG_LOCAL_FILTER TLOG_I
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
#define tlog_hexdump(level, info, ptr, len) tlog_rawhexdump(__FILE__, __LINE__, __func__, TLOG_LOCAL_FILTER, level, info, ptr, len);

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
	 * @brief 尝试手动滚动日志文件
	 * 日志文件会自动滚动，只有在希望提前滚动日志文件的时候手动调用本函数
	 * 
	 * @param limit B, 限制单个日志文件大小
	 */
	void tlog_try_rotate(ti64 limit);

	/**
	 * @brief 运行本线程滚动日志文件
	 * 
	 */
	void tlog_enable_local_rotate();

	/**
	 * @brief 禁止本线程滚动日志文件
	 * 
	 */
	void tlog_disable_local_rotate();

	/**
	 * @brief 跨平台，得到文件的basename
	 * 
	 * @param path 
	 * @return 
	 */
	tc *tlog_basename(const tc *path);

	/**
	 * @brief 跨平台, system
	 * 
	 * @param cmd 
	 * @return 
	 */
	ti tlog_system(const tc *cmd);

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

#ifdef __cplusplus
}
#endif