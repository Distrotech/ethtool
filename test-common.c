/****************************************************************************
 * Common test functions for ethtool
 * Copyright 2011 Solarflare Communications Inc.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 2 as published
 * by the Free Software Foundation, incorporated herein by reference.
 */

#include <stdlib.h>
#include <string.h>
#include <sys/fcntl.h>
#include <sys/wait.h>
#include <unistd.h>
#include "internal.h"

int test_cmdline(const char *args)
{
	int argc, i;
	char **argv;
	const char *arg;
	size_t len;
	pid_t pid;
	int dev_null;
	int status;
	int rc;

	/* Convert line to argv */
	argc = 1;
	arg = args;
	for (;;) {
		len = strcspn(arg, " ");
		if (len == 0)
			break;
		argc++;
		if (arg[len] == 0)
			break;
		arg += len + 1;
	}
	argv = calloc(argc + 1, sizeof(argv[0]));
	argv[0] = strdup("ethtool");
	arg = args;
	for (i = 1; i < argc; i++) {
		len = strcspn(arg, " ");
		argv[i] = malloc(len + 1);
		memcpy(argv[i], arg, len);
		argv[i][len] = 0;
		arg += len + 1;
	}

	dev_null = open("/dev/null", O_RDWR);
	if (dev_null < 0) {
		perror("open /dev/null");
		rc = -1;
		goto out;
	}

	fflush(NULL);
	pid = fork();

	/* Child */
	if (pid == 0) {
		dup2(dev_null, STDIN_FILENO);
		if (!getenv("ETHTOOL_TEST_VERBOSE")) {
			dup2(dev_null, STDOUT_FILENO);
			dup2(dev_null, STDERR_FILENO);
		}
		execv("./test-one-cmdline", argv);
		_exit(126);
	}

	/* Parent */
	if (pid < 0) {
		perror("fork");
		close(dev_null);
		rc = -1;
		goto out;
	}
	close(dev_null);
	if (waitpid(pid, &status, 0) < 0) {
		perror("waitpid");
		rc = -1;
		goto out;
	}
	rc = WIFEXITED(status) ? WEXITSTATUS(status) : -1;

out:
	for (i = 0; i < argc; i++)
		free(argv[i]);
	free(argv);
	return rc;
}
