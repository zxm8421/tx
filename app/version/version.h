/**
 * @brief 
 * 
 * @file version.h
 */
#pragma once

#include <tjz/ttype.h>

// qmake 会自动生成版本信息
#ifndef buildTime
#pragma message("编译器未生成buildTime")
#define buildTime 0LL
#endif
#ifndef buildSalt
#define buildSalt "000000"
#endif
#ifndef buildBranch
#pragma message("编译器未生成buildBranch")
#define buildBranch "none"
#endif
#ifndef buildSHA1
#pragma message("编译器未生成buildSHA1")
#define buildSHA1 "0000000"
#endif

#ifndef buildTarget
#pragma message("编译器未生成buildTarget")
#define buildTarget "app"
#endif

#ifndef buildVer_Major
#define buildVer_Major 0
#endif
#ifndef buildVer_Minor
#define buildVer_Minor 0
#endif
#ifndef buildVer_Patch
#define buildVer_Patch 0
#endif

#define buildVer (buildVer_Major * 1000 * 1000 + buildVer_Minor * 1000 + buildVer_Patch)

/**
 * @brief 显示版本号以及编译信息
 * 
 * @return 
 */
ti showVer();
