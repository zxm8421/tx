/**
 * @brief 
 * 
 * @file tsh.c
 */
#include "tsh.in.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <time.h>
#include <unistd.h>
#define __USE_GNU
#include <pthread.h>
static pthread_t tsh_tid = -1;

#undef TLOG_LOCAL_FILTER
#define TLOG_LOCAL_FILTER TLOG_D

extern struct tsh_Cmd *__start_tsh_cmd_vt;
extern struct tsh_Cmd *__stop_tsh_cmd_vt;

tc tsh_prompt[4096] = "tsh)";
ti tsh_ret = 0;

TSH_CMD_DEFINE(test, "测试")
{
	printf("tsh test\n");
	printf("tsh 测试\n");

	return 0;
}

TSH_CMD_DEFINE(help, "显示所有命令")
{
	for (struct tsh_Cmd **iter = &__start_tsh_cmd_vt; iter < &__stop_tsh_cmd_vt; iter++)
	{
		if ((*iter)->cmd_en == tbt)
		{
			printf("%-20s : %s\n", (*iter)->cmd_name, (*iter)->cmd_desc);
		}
	}

	return 0;
}

TSH_CMD_DEFINE(echo, "")
{
	for (ti i = 1; i < argc; i++)
	{
		if (strcmp(argv[i], "$0") == 0)
		{
			printf("tsh");
		}
		else if (strcmp(argv[i], "$?") == 0)
		{
			printf("%d", tsh_ret);
		}
		else
		{
			printf("%s", argv[i]);
		}

		if (i != (argc - 1))
		{
			printf(" ");
		}
		else
		{
			printf("\n");
		}
	}

	return 0;
}

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
		ti argc;
		tc *argv[256];

		ti len;
		tc buf[4096];
	} cmd;
	memset(&cmd, 0, sizeof(cmd));

	while (true)
	{
		{
			// ti64 now = tlog_getTimeMs();
			// ti64 ms = (now / 50 + 1) * 50 - now;
			// usleep(1000 * ms);
			static struct timespec tp;
			clock_nanosleep(CLOCK_MONOTONIC, TIMER_ABSTIME, &tp, NULL);
			clock_gettime(CLOCK_MONOTONIC, &tp);
			tp.tv_nsec = (tp.tv_nsec / (1000 * 1000 * 50) + 1) * (1000 * 1000 * 50);
		}
		ti len = read(STDIN_FILENO, cmd.buf + cmd.len, tx_array_size(cmd.buf) - cmd.len);
		if (len < 0)
		{
			tlog(TLOG_D, "len = %d", len);
			continue;
		}
		cmd.len += len;

		tlog(TLOG_D, "read %d, cmd.buf[%d] = %s", len, cmd.len, cmd.buf);
		tlog_hexdump(TLOG_D, "cmd.buf", cmd.buf, cmd.len);

		tc *token = strstr(cmd.buf, "\n");
		if (token != tnull)
		{
			*token = '\0';
			cmd.len = token - cmd.buf;

			if (cmd.len > 0)
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
					if (((*iter)->cmd_en == tbt) && (cmd.hash == (*iter)->cmd_hash) && (strcmp(cmd.argv[0], (*iter)->cmd_name) == 0))
					{
						tsh_ret = (*iter)->cmd_exec((const ti)cmd.argc, (const tc **)cmd.argv);
						// tlog(TLOG_D, "ret = %d", cmd.ret);
						// tlog_hexdump(TLOG_D, "cmd.buf", cmd.buf, cmd.len);
						break;
					}
				}

				if (iter == &__stop_tsh_cmd_vt)
				{
					tlog(TLOG_W, "未找到命令");
				}
			}

			memset(&cmd, 0, sizeof(cmd));
			write(STDOUT_FILENO, tsh_prompt, sizeof(tsh_prompt));
		}
	}

	return NULL;
}

int tsh_cmd_cmp(const void *a, const void *b)
{
	struct tsh_Cmd **iter_a = (struct tsh_Cmd **)a;
	struct tsh_Cmd **iter_b = (struct tsh_Cmd **)b;

	return strcmp((*iter_a)->cmd_name, (*iter_b)->cmd_name);
}

ti tsh_init()
{
	tlog(TLOG_D, "&__stop_tsh_cmd_vt - &__start_tsh_cmd_vt = %d", &__stop_tsh_cmd_vt - &__start_tsh_cmd_vt);
	qsort(&__start_tsh_cmd_vt, &__stop_tsh_cmd_vt - &__start_tsh_cmd_vt - 1, sizeof(void *), &tsh_cmd_cmp);
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
