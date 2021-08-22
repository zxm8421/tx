/**
 * @brief 
 * 
 * @file version.h
 */
#pragma once

#include <tjz/ttype.h>

#include <QVersionNumber>
#include <QDateTime>

/**
 * @brief 显示版本号以及编译信息
 * 
 * @return 
 */
ti showVer();

/**
 * @brief 获取编译目标
 * 
 * @return 
 */
QString getBuildTarget();

/**
 * @brief 获取版本号
 * 
 * @return 
 */
QVersionNumber getVersionNumber();

/**
 * @brief 获取编译时间
 * 
 * @return 
 */
QDateTime getBuildTime();

/**
 * @brief 获取编译盐
 * 
 * @return 
 */
QString getBuildSalt();

/**
 * @brief 获取编译盐
 * 
 * @return 
 */
constexpr tu64 getBuildSalt_u64();

/**
 * @brief 获取编译分支
 * 
 * @return 
 */
QString getBuildBranch();

/**
 * @brief 获取编译commit sha1
 * 
 * @return 
 */
QString getBuildSHA1();