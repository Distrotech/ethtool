/****************************************************************************
 * Test cases for ethtool features
 * Copyright 2012 Solarflare Communications Inc.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 2 as published
 * by the Free Software Foundation, incorporated herein by reference.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define TEST_NO_WRAPPERS
#include "internal.h"

static const struct ethtool_value
cmd_grxcsum_off = { ETHTOOL_GRXCSUM, 0 },
cmd_grxcsum_on = { ETHTOOL_GRXCSUM, 1 },
cmd_srxcsum_off = { ETHTOOL_SRXCSUM, 0 },
cmd_srxcsum_on = { ETHTOOL_SRXCSUM, 1 },
cmd_gtxcsum_off = { ETHTOOL_GTXCSUM, 0 },
cmd_gtxcsum_on = { ETHTOOL_GTXCSUM, 1 },
cmd_stxcsum_off = { ETHTOOL_STXCSUM, 0 },
cmd_stxcsum_on = { ETHTOOL_STXCSUM, 1 },
cmd_gsg_off = { ETHTOOL_GSG, 0 },
cmd_gsg_on = { ETHTOOL_GSG, 1 },
cmd_ssg_off = { ETHTOOL_SSG, 0 },
cmd_ssg_on = { ETHTOOL_SSG, 1 },
cmd_gtso_off = { ETHTOOL_GTSO, 0 },
cmd_gtso_on = { ETHTOOL_GTSO, 1 },
cmd_stso_off = { ETHTOOL_STSO, 0 },
cmd_stso_on = { ETHTOOL_STSO, 1 },
cmd_gufo_off = { ETHTOOL_GUFO, 0 },
cmd_gufo_on = { ETHTOOL_GUFO, 1 },
cmd_sufo_off = { ETHTOOL_SUFO, 0 },
cmd_sufo_on = { ETHTOOL_SUFO, 1 },
cmd_ggso_off = { ETHTOOL_GGSO, 0 },
cmd_ggso_on = { ETHTOOL_GGSO, 1 },
cmd_sgso_off = { ETHTOOL_SGSO, 0 },
cmd_sgso_on = { ETHTOOL_SGSO, 1 },
cmd_ggro_off = { ETHTOOL_GGRO, 0 },
cmd_ggro_on = { ETHTOOL_GGRO, 1 },
cmd_sgro_off = { ETHTOOL_SGRO, 0 },
cmd_sgro_on = { ETHTOOL_SGRO, 1 },
cmd_gflags_off = { ETHTOOL_GFLAGS, 0 },
cmd_gflags_on = { ETHTOOL_GFLAGS,
		  ETH_FLAG_LRO | ETH_FLAG_RXVLAN | ETH_FLAG_TXVLAN |
		  ETH_FLAG_NTUPLE | ETH_FLAG_RXHASH },
cmd_sflags_off = { ETHTOOL_SFLAGS, 0 },
cmd_sflags_ntuple = { ETHTOOL_GFLAGS, ETH_FLAG_NTUPLE },
cmd_sflags_on = { ETHTOOL_SFLAGS,
		  ETH_FLAG_LRO | ETH_FLAG_RXVLAN | ETH_FLAG_TXVLAN |
		  ETH_FLAG_NTUPLE | ETH_FLAG_RXHASH },
cmd_sflags_not_rxhash = { ETHTOOL_SFLAGS,
			  ETH_FLAG_LRO | ETH_FLAG_RXVLAN | ETH_FLAG_TXVLAN |
			  ETH_FLAG_NTUPLE };

static const struct cmd_expect cmd_expect_get_features_off[] = {
	{ &cmd_grxcsum_off, 4, 0, &cmd_grxcsum_off, sizeof(cmd_grxcsum_off) },
	{ &cmd_gtxcsum_off, 4, 0, &cmd_gtxcsum_off, sizeof(cmd_gtxcsum_off) },
	{ &cmd_gsg_off, 4, 0, &cmd_gsg_off, sizeof(cmd_gsg_off) },
	{ &cmd_gtso_off, 4, 0, &cmd_gtso_off, sizeof(cmd_gtso_off) },
	{ &cmd_gufo_off, 4, 0, &cmd_gufo_off, sizeof(cmd_gufo_off) },
	{ &cmd_ggso_off, 4, 0, &cmd_ggso_off, sizeof(cmd_ggso_off) },
	{ &cmd_ggro_off, 4,0, &cmd_ggro_off, sizeof(cmd_ggro_off) },
	{ &cmd_gflags_off, 4, 0, &cmd_gflags_off, sizeof(cmd_gflags_off) },
	{ 0, 0, 0, 0, 0 }
};

static const struct cmd_expect cmd_expect_set_features_off[] = {
	{ &cmd_grxcsum_on, 4, 0, &cmd_grxcsum_on, sizeof(cmd_grxcsum_on) },
	{ &cmd_gtxcsum_on, 4, 0, &cmd_gtxcsum_on, sizeof(cmd_gtxcsum_on) },
	{ &cmd_gsg_on, 4, 0, &cmd_gsg_on, sizeof(cmd_gsg_on) },
	{ &cmd_gtso_on, 4, 0, &cmd_gtso_on, sizeof(cmd_gtso_on) },
	{ &cmd_gufo_on, 4, 0, &cmd_gufo_on, sizeof(cmd_gufo_on) },
	{ &cmd_ggso_on, 4, 0, &cmd_ggso_on, sizeof(cmd_ggso_on) },
	{ &cmd_ggro_on, 4,0, &cmd_ggro_on, sizeof(cmd_ggro_on) },
	{ &cmd_gflags_on, 4, 0, &cmd_gflags_on, sizeof(cmd_sflags_on) },
	{ &cmd_srxcsum_off, sizeof(cmd_srxcsum_off), 0, 0, 0 },
	{ &cmd_stxcsum_off, sizeof(cmd_stxcsum_off), 0, 0, 0 },
	{ &cmd_ssg_off, sizeof(cmd_ssg_off), 0, 0, 0 },
	{ &cmd_stso_off, sizeof(cmd_stso_off), 0, 0, 0 },
	{ &cmd_sufo_off, sizeof(cmd_sufo_off), 0, 0, 0 },
	{ &cmd_sgso_off, sizeof(cmd_sgso_off), 0, 0, 0 },
	{ &cmd_sgro_off, sizeof(cmd_sgro_off), 0, 0, 0 },
	{ &cmd_sflags_off, sizeof(cmd_sflags_off), 0, 0, 0 },
	{ &cmd_grxcsum_off, 4, 0, &cmd_grxcsum_off, sizeof(cmd_grxcsum_off) },
	{ &cmd_gtxcsum_off, 4, 0, &cmd_gtxcsum_off, sizeof(cmd_gtxcsum_off) },
	{ &cmd_gsg_off, 4, 0, &cmd_gsg_off, sizeof(cmd_gsg_off) },
	{ &cmd_gtso_off, 4, 0, &cmd_gtso_off, sizeof(cmd_gtso_off) },
	{ &cmd_gufo_off, 4, 0, &cmd_gufo_off, sizeof(cmd_gufo_off) },
	{ &cmd_ggso_off, 4, 0, &cmd_ggso_off, sizeof(cmd_ggso_off) },
	{ &cmd_ggro_off, 4,0, &cmd_ggro_off, sizeof(cmd_ggro_off) },
	{ &cmd_gflags_off, 4, 0, &cmd_gflags_off, sizeof(cmd_sflags_off) },
	{ 0, 0, 0, 0, 0 }
};

static const struct cmd_expect cmd_expect_set_features_on[] = {
	{ &cmd_grxcsum_off, 4, 0, &cmd_grxcsum_off, sizeof(cmd_grxcsum_off) },
	{ &cmd_gtxcsum_off, 4, 0, &cmd_gtxcsum_off, sizeof(cmd_gtxcsum_off) },
	{ &cmd_gsg_off, 4, 0, &cmd_gsg_off, sizeof(cmd_gsg_off) },
	{ &cmd_gtso_off, 4, 0, &cmd_gtso_off, sizeof(cmd_gtso_off) },
	{ &cmd_gufo_off, 4, 0, &cmd_gufo_off, sizeof(cmd_gufo_off) },
	{ &cmd_ggso_off, 4, 0, &cmd_ggso_off, sizeof(cmd_ggso_off) },
	{ &cmd_ggro_off, 4,0, &cmd_ggro_off, sizeof(cmd_ggro_off) },
	{ &cmd_gflags_off, 4, 0, &cmd_gflags_off, sizeof(cmd_gflags_off) },
	{ &cmd_srxcsum_on, sizeof(cmd_srxcsum_on), 0, 0, 0 },
	{ &cmd_stxcsum_on, sizeof(cmd_stxcsum_on), 0, 0, 0 },
	{ &cmd_ssg_on, sizeof(cmd_ssg_on), 0, 0, 0 },
	{ &cmd_stso_on, sizeof(cmd_stso_on), 0, 0, 0 },
	{ &cmd_sufo_on, sizeof(cmd_sufo_on), 0, 0, 0 },
	{ &cmd_sgso_on, sizeof(cmd_sgso_on), 0, 0, 0 },
	{ &cmd_sgro_on, sizeof(cmd_sgro_on), 0, 0, 0 },
	{ &cmd_sflags_on, sizeof(cmd_sflags_on), 0, 0, 0 },
	{ &cmd_grxcsum_on, 4, 0, &cmd_grxcsum_on, sizeof(cmd_grxcsum_on) },
	{ &cmd_gtxcsum_on, 4, 0, &cmd_gtxcsum_on, sizeof(cmd_gtxcsum_on) },
	{ &cmd_gsg_on, 4, 0, &cmd_gsg_on, sizeof(cmd_gsg_on) },
	{ &cmd_gtso_on, 4, 0, &cmd_gtso_on, sizeof(cmd_gtso_on) },
	{ &cmd_gufo_on, 4, 0, &cmd_gufo_on, sizeof(cmd_gufo_on) },
	{ &cmd_ggso_on, 4, 0, &cmd_ggso_on, sizeof(cmd_ggso_on) },
	{ &cmd_ggro_on, 4,0, &cmd_ggro_on, sizeof(cmd_ggro_on) },
	{ &cmd_gflags_on, 4, 0, &cmd_gflags_on, sizeof(cmd_sflags_on) },
	{ 0, 0, 0, 0, 0 }
};

static struct test_case {
	int rc;
	const char *args;
	const struct cmd_expect *expect;
} const test_cases[] = {
	{ 0, "-k devname", cmd_expect_get_features_off },
	{ 0, "-K devname rx off tx off sg off tso off ufo off gso off lro off rxvlan off txvlan off ntuple off rxhash off gro off",
	  cmd_expect_set_features_off },
	{ 0, "-K devname rx on tx on sg on tso on ufo on gso on lro on rxvlan on txvlan on ntuple on rxhash on gro on",
	  cmd_expect_set_features_on },
};

static int expect_matched;
static const struct cmd_expect *expect_next;

int send_ioctl(struct cmd_context *ctx, void *cmd)
{
	int rc = test_ioctl(expect_next, cmd);

	if (rc == TEST_IOCTL_MISMATCH) {
		expect_matched = 0;
		test_exit(0);
	}
	expect_next++;
	return rc;
}

int main(void)
{
	const struct test_case *tc;
	int test_rc;
	int rc = 0;

	for (tc = test_cases; tc < test_cases + ARRAY_SIZE(test_cases); tc++) {
		if (getenv("ETHTOOL_TEST_VERBOSE"))
			printf("I: Test command line: ethtool %s\n", tc->args);
		expect_matched = 1;
		expect_next = tc->expect;
		test_rc = test_cmdline(tc->args);

		/* If we found a mismatch, or there is still another
		 * expected ioctl to match, the test failed.
		 */
		if (!expect_matched || expect_next->cmd) {
			fprintf(stderr,
				"E: ethtool %s deviated from the expected "
				"ioctl sequence after %zu calls\n",
				tc->args, expect_next - tc->expect);
			rc = 1;
		} else if (test_rc != tc->rc) {
			fprintf(stderr, "E: ethtool %s returns %d\n",
				tc->args, test_rc);
			rc = 1;
		}
	}

	return rc;
}
