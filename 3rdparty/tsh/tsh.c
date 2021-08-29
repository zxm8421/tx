#include "tsh.inc.h"

#include <stdio.h>
#include <string.h>

#include <unistd.h>
#define __USE_GNU
#include <pthread.h>
static pthread_t tsh_tid = -1;

#define TLOG_LOCAL_FILTER TLOG_D
#include <tlog/tlog.h>

extern struct tsh_Cmd *__start_tsh_cmd_vt;
extern struct tsh_Cmd *__stop_tsh_cmd_vt;

tc tsh_prompt[4096] = "tsh)";

TSH_CMD_DEFINE(test)
{
	printf("tsh test\n");
	printf("tsh 测试\n");

	return 0;
}
TSH_CMD_EXPORT(test, "tsh test 测试");

// FNV hash, http://www.isthe.com/chongo/tech/comp/fnv/index.html
tu32 tsh_hash(const tc *str)
{
	const tu32 basis = 2166136261;
	const tu32 prime = 16777619;

	tu32 hash = basis;
	tc *cur = (tc *)str;

	while (*cur)
	{
		hash ^= *cur;
		hash *= prime;
		cur++;
	}

	return hash;
}

void *tsh_thread_run(void *arg __attribute__((unused)))
{
	struct
	{
		tu32 hash;
		ti ret;
		ti argc;
		tc *argv[256];

		ti len;
		tc buf[4096];
	} cmd;
	memset(&cmd, 0, sizeof(cmd));

	while (true)
	{
		usleep(1000 * 20);
		ti len = read(STDIN_FILENO, cmd.buf + cmd.len, tx_array_size(cmd.buf) - cmd.len);
		if (len < 0)
		{
			tlog(TLOG_D, "len = %d", len);
			continue;
		}

		cmd.len += len;

		tlog(TLOG_D, "read %d, cmd.buf[%d] = %s", len, cmd.len, cmd.buf);
		tlog_hexdump(TLOG_D, "cmd.buf", cmd.buf, cmd.len);

		if (strtok(cmd.buf, "\n") != tnull)
		{
			tc *token = strtok(cmd.buf, " ");
			while (token)
			{
				cmd.argv[cmd.argc++] = token;
				token = strtok(NULL, " ");

				tlog(TLOG_D, "cmd.argv[%d] = %s", cmd.argc - 1, cmd.argv[cmd.argc - 1]);
			}

			cmd.hash = tsh_hash(cmd.argv[0]);

			struct tsh_Cmd **iter = tnull;
			for (iter = &__start_tsh_cmd_vt; iter < &__stop_tsh_cmd_vt; iter++)
			{
				if ((cmd.hash == (*iter)->cmd_hash) && (strcmp(cmd.argv[0], (*iter)->cmd_name) == 0))
				{
					cmd.ret = (*iter)->cmd_exec((const ti)cmd.argc, (const tc **)cmd.argv);
					tlog(TLOG_D, "ret = %d", cmd.ret);
					tlog_hexdump(TLOG_D, "cmd.buf", cmd.buf, cmd.len);
					break;
				}
			}

			if (iter == &__stop_tsh_cmd_vt)
			{
				tc s[] = "未找到命令\n";
				write(STDOUT_FILENO, s, sizeof(s));
			}
		}

		memset(&cmd, 0, sizeof(cmd));

		write(STDOUT_FILENO, tsh_prompt, sizeof(tsh_prompt));
	}

	return NULL;
}

ti tsh_init()
{
	tlog(TLOG_D, "&__stop_tsh_cmd_vt - &__start_tsh_cmd_vt = %d", &__stop_tsh_cmd_vt - &__start_tsh_cmd_vt);
	for (struct tsh_Cmd **iter = &__start_tsh_cmd_vt; iter < &__stop_tsh_cmd_vt; iter++)
	{
		(*iter)->cmd_hash = tsh_hash((*iter)->cmd_name);
		tlog(TLOG_D, "iter = %p", iter);
		tlog(TLOG_D, "*iter = %p", *iter);
		tlog(TLOG_D, "(*iter)->cmd_name = %s, (*iter)->cmd_desc = %s", (*iter)->cmd_name, (*iter)->cmd_desc);
		tlog(TLOG_D, "(*iter)->cmd_hash = %u", (*iter)->cmd_hash);
	}

	pthread_create(&tsh_tid, NULL, tsh_thread_run, tnull);
	pthread_setname_np(tsh_tid, "tsh");

	tlog(TLOG_D, "当前 pid = %d, tid = %p", getpid(), pthread_self());
	tlog(TLOG_D, "创建tsh线程 tid = %p", tsh_tid);

	return 0;
}
