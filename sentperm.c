/*-
 * Copyright (c) 2009 Wojciech A. Koszek <wkoszek@FreeBSD.org>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 * $Id$
 */


#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#ifdef __linux__
#include <getopt.h>
#endif

static int	g_debug;

struct sent {
	char	*st_text;
	char	*st_cmt;
};
#define STM_TYPE	unsigned long
#define STM_MAX		(sizeof(STM_TYPE) * 8)

#define	ASSERT		assert

static int
stm_read_file(struct sent *sts, int sts_max)
{
	char buf[512];
	char *cmt;
	char *data;
	char *line;
	char *lineend;
	int si;

	cmt = data = NULL;
	line = lineend = NULL;

	/* Read sentences and comments */
	for (si = 0; si < sts_max; ) {
		line = fgets(buf, sizeof(buf), stdin);
		if (line == NULL)
			break;
		while (isspace(*line) && *line != '\0')
			line++;
		lineend = strrchr(line, '\0');
		assert(lineend != NULL);
		lineend--;
		while (isspace(*lineend)) {
			*lineend = '\0';
			lineend--;
		}
		if (strlen(line) < 1)
			break;
		data = strdup(line);
		assert(data != NULL);
		sts[si].st_text = data;
		cmt = strchr(data, '#');
		if (cmt != NULL) {
			*cmt = '\0';
			cmt++;
			sts[si].st_cmt = cmt;
		} else {
			sts[si].st_cmt = NULL;
		}
		si++;
	}
	return (si);
}

static int
stm_read_argv(struct sent *sts, int sts_max, int argc, char **argv)
{

	(void)sts;
	(void)sts_max;
	(void)argc;
	(void)argv;
	return (0);
}

static void
usage(void)
{

	(void)fprintf(stderr, "sentperm [-dh] [arg0 [arg1 ...]]\n");
	exit(EXIT_SUCCESS);
}

int
main(int argc, char **argv)
{
	STM_TYPE stmi;
	STM_TYPE stm;
	struct sent sts[STM_MAX];
	int bi, i, si, o;

	bi = i = si = 0;

	g_debug = 0;
	while ((o = getopt(argc, argv, "hd")) != -1)
		switch (o) {
		case 'd':
			g_debug++;
			break;
		case 'h':
			usage();
			break;
		}
	argc--;
	argv++;

	if (argc >= 1) {
		si = stm_read_argv(sts, STM_MAX, argc, argv);
	} else {
		si = stm_read_file(sts, STM_MAX);
	}

	if (g_debug) {
		printf("# sentences read: %d\n", si);
	}

	if (si == 0)
		return (EXIT_SUCCESS);

	/* 
	 * Set a bit for each sentence read. Sentence becomes the owner
	 * of a bit
	 */
	stm = 0;
	for (i = 0; i < si; i++)
		stm |= (1 << i);

	/* Generate nice output after ^D */
	if (isatty(STDIN_FILENO))
		puts("");

	/* Iterate for all bits set */
	for (stmi = 0; stmi <= stm; stmi++) {
		/* Check only bits with owners */
		for (bi = 0; bi < si; bi++) {
			if (stmi & (1 << bi))
				printf(" %s ", sts[bi].st_text);
			else
				printf("!%s ", sts[bi].st_text);
		}
		printf("\n");
	}

	exit(EXIT_SUCCESS);
}
