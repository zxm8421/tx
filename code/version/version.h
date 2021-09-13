/**
 * @file version.h
 * 
 * @brief 
 * 
 */
#pragma once

#include <tlib.h>

#include <QDateTime>
#include <QVersionNumber>

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
