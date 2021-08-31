/**
 * @brief 
 * 
 * @file tlog.inc.h
 */
#pragma once
#include "tlog.h"

#ifdef __cplusplus
extern "C"
{
#endif
	/**
	 * @brief tlog自用, 打印函数
	 * 用于tlog没起来的场合，以及tlog调试自己
	 * 会在末尾添加'\\n'
	 * 
	 * @param format 
	 * @param ... 
	 * @return 
	 */
	ti tlog_debug(const tc *format, ...);

#ifdef __cplusplus
}
#endif
