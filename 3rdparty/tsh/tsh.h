/**
 * @brief 
 * 
 * @file tsh.h
 */
#pragma once
#include <tjz/ttype.h>

struct tsh_Cmd
{
	ti (*cmd_exec)(__attribute__((unused)) const ti argc, __attribute__((unused)) const tc *argv[]);
	tc *cmd_name;
	tc *cmd_desc;
	tu32 cmd_hash;
};

#define TSH_CMD_DEFINE(cmd) ti cmd(__attribute__((unused)) const ti argc, __attribute__((unused)) const tc *argv[])
#define TSH_CMD_EXPORT(cmd, desc)        \
	struct tsh_Cmd tsh_cmd_sym_##cmd = { \
		.cmd_exec = cmd,                 \
		.cmd_name = (tc *)#cmd,          \
		.cmd_desc = (tc *)desc,          \
		.cmd_hash = 0,                   \
	};                                   \
	__attribute__((section("tsh_cmd_vt"), aligned(sizeof(void *)))) struct tsh_Cmd *tsh_cmd_psym_##cmd = &tsh_cmd_sym_##cmd;

#ifdef __cplusplus
extern "C"
{
#endif

	ti tsh_init();
	tu32 tsh_hash(const tc *str);
	ti tsh_cmd_enable(const tc *cmd);
	ti tsh_cmd_disable(const tc *cmd);

#ifdef __cplusplus
}
#endif
