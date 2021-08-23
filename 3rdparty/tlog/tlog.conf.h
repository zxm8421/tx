/**
 * @brief 
 * 
 * @file tlog.conf.h
 */
#pragma once

/**
 * @brief 配置项
 * 
 */
#define TLOG_USE_UTC 0
#define TLOG_BUF_SIZE 4096
#define TLOG_CONSOLE_ENABLE 1
#define TLOG_FILE_ENABLE 1
#define TLOG_FILE_NUM 10
#define TLOG_FILE_SIZE (10 * 1024 * 1024)
#define TLOG_FILE_DIR "log"
#define TLOG_FILE_PREFIX "tx"
