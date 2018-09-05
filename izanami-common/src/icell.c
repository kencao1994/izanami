/*
 * icell.c
 *
 *  Created on: 2018年9月4日
 *      Author: caojx
 */

#include "config.h"
#include "icell.h"

#include <string.h>

struct icell *fromptr(void *ptr) {

	return (struct icell *)ptr;
}

int icellsize(struct icell *icell) {

	return 3 * sizeof(short) + icell->colen + icell->keylen +icell->vallen;
}

static char buf1[IZANAMI_MAX_LEN];
static char buf2[IZANAMI_MAX_LEN];

void lencpy(char *to, char *from, short len) {

	short tmp = 0;
	while (tmp < len) {
		to[tmp] = from[tmp];
		tmp ++;
	}
}

int icellcmp(struct icell *arg1, struct icell *arg2) {

	int ret = 0;



	return ret;
}
