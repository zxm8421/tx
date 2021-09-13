/**
 * @brief 
 * 
 * @file tzero.h
 */
#pragma once
#include <tlib.h>

#include <stdarg.h>

/*******************************************************************************
 * @par export
 */
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
	ti tlib_system(const tc *cmd, ...);

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

	// tb8 tlib_tb8_set();
	// tb8 tlib_tb8_reset();

	// // 注意入参类型提升
	// typedef ti (*tlib_format_Handle)(tc *buf, const struct tlib_format_info *info, va_list ap);

	// ti tlib_format(tc *buf, const tc *format, ...);

	// ti tlib_vformat(tc *buf, const tc *format, va_list ap);
	// ti tlib_format_register(tc *spec, tlib_format_Handle handle);

	// // less is more
	// // "{:u8}"
	// // "{:u8*:}"
	// // "{:u8[*]:}"
	// // "{:u8[12]:}"
	// // "{:u8:^ }"
	// // "{:u8[*]:^+03.4}"
	// // "{:u8[10_]:^+03.4}"
	// // "{:u8[*,]e:^+03.4}"
	// // "{:u8[*,]#x:^+03.4}"
	// // "{:time::YYYYMMDD.HHMMSS}"
	// // 转义 // /{ /} /:	/[ /]
	// struct tlib_format_info
	// {
	// 	ti pos;	 // 参数位置
	// 	ti size; // 参数类型注册的大小

	// 	tc *spec;	// 类型说明符
	// 	tc hashtag; // #扩展, [\0#]
	// 	tc alt;		// 类型辅助说明符 [\0eEbBoOdDxX]

	// 	tc array;	 // 是否是数组 [\0*]
	// 	ti arrayLen; // 数组元素个数
	// 	tc gap;		 // 数组各元素之间的间隔字符, 最后一个元素后无此字符, 默认[ ],可以为 [ ,_]

	// 	tc align; // 对齐方式, [\0<^>]
	// 	tc sign;  // 符号, [\0+-]
	// 	tc pad;	  // 填充字符 [\0 0_]
	// 	ti width; // 宽度
	// 	ti prec;  // 精度

	// 	tc *extra; // 自定义选项
	// };
	// int i = sizeof(struct tlib_format_info);
	// ti tlib_vformat_u8(tc *buf, const struct tlib_format_info *info, va_list ap);
	// ti tlib_vformat_i8(tc *buf, const struct tlib_format_info *info, va_list ap);

	// typedef ti (*tlib_parse_handle)(const tc *buf, const struct tlib_format_info *info, void *args);

	// ti tlib_parse(tc *buf, tc *format, ...);

#ifdef __cplusplus
}
#endif
