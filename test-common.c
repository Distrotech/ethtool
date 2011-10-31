/****************************************************************************
 * Common test functions for ethtool
 * Copyright 2011 Solarflare Communications Inc.
 *
 * Partly derived from kernel <linux/list.h>.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 2 as published
 * by the Free Software Foundation, incorporated herein by reference.
 */

#include <assert.h>
#include <setjmp.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <sys/fcntl.h>
#include <unistd.h>
#define TEST_NO_WRAPPERS
#include "internal.h"

/* List utilities */

struct list_head {
	struct list_head *next, *prev;
};

#define LIST_HEAD_INIT(name) { &(name), &(name) }

static void init_list_head(struct list_head *list)
{
	list->next = list;
	list->prev = list;
}

static void list_add(struct list_head *new, struct list_head *head)
{
	head->next->prev = new;
	new->next = head->next;
	new->prev = head;
	head->next = new;
}

static void list_del(struct list_head *entry)
{
	entry->next->prev = entry->prev;
	entry->prev->next = entry->next;
	entry->next = NULL;
	entry->prev = NULL;
}

#define list_for_each_safe(pos, n, head) \
	for (pos = (head)->next, n = pos->next; pos != (head); \
		pos = n, n = pos->next)

/* Free memory at end of test */

static struct list_head malloc_list = LIST_HEAD_INIT(malloc_list);

void *test_malloc(size_t size)
{
	struct list_head *block = malloc(sizeof(*block) + size);

	if (!block)
		return NULL;
	list_add(block, &malloc_list);
	return block + 1;
}

void *test_calloc(size_t nmemb, size_t size)
{
	void *ptr = test_malloc(nmemb * size);

	if (ptr)
		memset(ptr, 0, nmemb * size);
	return ptr;
}

char *test_strdup(const char *s)
{
	size_t size = strlen(s) + 1;
	char *dup = test_malloc(size);

	if (dup)
		memcpy(dup, s, size);
	return dup;
}

void test_free(void *ptr)
{
	struct list_head *block;

	if (!ptr)
		return;
	block = (struct list_head *)ptr - 1;
	list_del(block);
	free(block);
}

void *test_realloc(void *ptr, size_t size)
{
	struct list_head *block;

	if (ptr) {
		block = (struct list_head *)ptr - 1;
		list_del(block);
	}
	block = realloc(block, sizeof(*block) + size);
	if (!block)
		return NULL;
	list_add(block, &malloc_list);
	return block + 1;
}

static void test_free_all(void)
{
	struct list_head *block, *next;

	list_for_each_safe(block, next, &malloc_list)
		free(block);
	init_list_head(&malloc_list);
}

/* Close files at end of test */

struct file_node {
	struct list_head link;
	FILE *fh;
	int fd;
};

static struct list_head file_list = LIST_HEAD_INIT(file_list);

int test_open(const char *pathname, int flag, ...)
{
	struct file_node *node;
	mode_t mode;

	if (flag & O_CREAT) {
		va_list ap;
		va_start(ap, flag);
		mode = va_arg(ap, mode_t);
		va_end(ap);
	} else {
		mode = 0;
	}

	node = malloc(sizeof(*node));
	if (!node)
		return -1;

	node->fd = open(pathname, flag, mode);
	if (node->fd < 0) {
		free(node);
		return -1;
	}

	node->fh = NULL;
	list_add(&node->link, &file_list);
	return node->fd;
}

int test_socket(int domain, int type, int protocol)
{
	struct file_node *node;

	node = malloc(sizeof(*node));
	if (!node)
		return -1;

	node->fd = socket(domain, type, protocol);
	if (node->fd < 0) {
		free(node);
		return -1;
	}

	node->fh = NULL;
	list_add(&node->link, &file_list);
	return node->fd;
}

int test_close(int fd)
{
	struct list_head *head, *next;

	if (fd >= 0) {
		list_for_each_safe(head, next, &file_list) {
			if (((struct file_node *)head)->fd == fd) {
				list_del(head);
				free(head);
				break;
			}
		}
	}

	return close(fd);
}

FILE *test_fopen(const char *path, const char *mode)
{
	struct file_node *node;

	node = malloc(sizeof(*node));
	if (!node)
		return NULL;

	node->fh = fopen(path, mode);
	if (!node->fh) {
		free(node);
		return NULL;
	}

	node->fd = -1;
	list_add(&node->link, &file_list);
	return node->fh;	
}

int test_fclose(FILE *fh)
{
	struct list_head *head, *next;

	assert(fh);

	list_for_each_safe(head, next, &file_list) {
		if (((struct file_node *)head)->fh == fh) {
			list_del(head);
			free(head);
			break;
		}
	}

	return fclose(fh);
}

static void test_close_all(void)
{
	struct list_head *head, *next;
	struct file_node *node;

	list_for_each_safe(head, next, &file_list) {
		node = (struct file_node *)head;
		if (node->fh)
			fclose(node->fh);
		else
			close(node->fd);
		free(node);
	}
	init_list_head(&file_list);
}

/* Wrap test main function */

static jmp_buf test_return;

void test_exit(int rc)
{
	longjmp(test_return, rc + 1);
}

int test_cmdline(const char *args)
{
	int argc, i;
	char **argv;
	const char *arg;
	size_t len;
	int dev_null = -1, old_stdout = -1, old_stderr = -1;
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
	argv = test_calloc(argc + 1, sizeof(argv[0]));
	argv[0] = test_strdup("ethtool");
	arg = args;
	for (i = 1; i < argc; i++) {
		len = strcspn(arg, " ");
		argv[i] = test_malloc(len + 1);
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
	dup2(dev_null, STDIN_FILENO);
	if (!getenv("TEST_TEST_VERBOSE")) {
		old_stdout = dup(STDOUT_FILENO);
		if (old_stdout < 0) {
			perror("dup stdout");
			rc = -1;
			goto out;
		}
		dup2(dev_null, STDOUT_FILENO);
		old_stderr = dup(STDERR_FILENO);
		if (old_stderr < 0) {
			perror("dup stderr");
			rc = -1;
			goto out;
		}
		dup2(dev_null, STDERR_FILENO);
	}

	rc = setjmp(test_return);
	rc = rc ? rc - 1 : test_main(argc, argv);

out:
	fflush(NULL);
	if (old_stderr >= 0) {
		dup2(old_stderr, STDERR_FILENO);
		close(old_stderr);
	}
	if (old_stdout >= 0) {
		dup2(old_stdout, STDOUT_FILENO);
		close(old_stdout);
	}
	if (dev_null >= 0)
		close(dev_null);

	test_free_all();
	test_close_all();
	return rc;
}
