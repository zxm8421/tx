/**
 * @brief 
 * 
 * @file version.h
 */
#pragma once

#include <tjz/ttype.h>

#ifndef buildTime
#define buildTime 0LL
#endif
#ifndef buildSHA1
#define buildSHA1 "0000000"
#endif

#define buildVer_Major 0
#define buildVer_Minor 1
#define buildVer_Patch 0
#define buildVer (buildVer_Major * 1000 * 1000 + buildVer_Minor * 1000 + buildVer_Patch)

/**
 * @brief 显示版本号以及编译信息
 * 
 * @return 
 */
ti showVer();
