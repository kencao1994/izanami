/*
 * icell.c
 *
 *  Created on: 2018年9月4日
 *      Author: caojx
 */

#include "block.h"
#include "config.h"
#include "icell.h"

#include <stdlib.h>
#include <string.h>

struct icell *fromptr(void *ptr) {

	return (struct icell *) ptr;
}

int icellheadlen(struct icell *icell) {

	return 3 * sizeof(short);
}

int icellsize(struct icell *icell) {

	return 3 * sizeof(short) + icell->collen + icell->keylen + icell->vallen
			+ sizeof(long);
}

static char buf1[IZANAMI_MAX_LEN];
static char buf2[IZANAMI_MAX_LEN];

void lencpy(char *to, char *from, short len) {

	short tmp = 0;
	while (tmp < len) {
		to[tmp] = from[tmp];
		tmp++;
	}
}

int icellcmp(void *left, void *right) {

	struct icell *arg1 = left;
	struct icell *arg2 = right;

	if (arg1 == NULL)
		return -1;
	if (arg2 == NULL)
		return 1;

	int ret = 0;

	getcol(arg1, buf1);
	getcol(arg2, buf2);
	ret = strcmp(buf1, buf2);
	if (ret != 0)
		return ret;

	getval(arg1, buf1);
	getval(arg2, buf2);
	ret = strcmp(buf1, buf2);
	if (ret != 0)
		return ret;

	getkey(arg1, buf1);
	getkey(arg2, buf2);
	ret = strcmp(buf1, buf2);
	if (ret != 0)
		return ret;

	long mvcc1 = getmvcc(arg1);
	long mvcc2 = getmvcc(arg2);

	return mvcc1 > mvcc2 ? 1 : -1;
}

long getmvcc(struct icell *icell) {

	return *(long *) ((void *) icell + icellheadlen(icell) + icell->collen
			+ icell->vallen + icell->keylen);
}

void getkey(struct icell *icell, char *buf) {

	memset(buf, 0, IZANAMI_MAX_LEN);
	memcpy(buf,
			(void *) icell + icellheadlen(icell) + icell->collen
					+ icell->vallen, icell->keylen);
}

void getcol(struct icell *icell, char *buf) {

	memset(buf, 0, IZANAMI_MAX_LEN);
	memcpy(buf, (void *) icell + icellheadlen(icell), icell->collen);
}

void getval(struct icell *icell, char *buf) {

	memset(buf, 0, IZANAMI_MAX_LEN);
	memcpy(buf, (void *) icell + icellheadlen(icell) + icell->collen,
			icell->vallen);
}

static long mvccnum = 0;

long generatemvcc() {

	return mvccnum++;
}

void randstr(char *buf, int len) {

	int tmp = 0;
	while (tmp < len) {
		buf[tmp] = rand() % 10 + '0';
		tmp++;
	}
}

struct icell *randicellfromcousumer(struct memconsumer *consumer) {

	short keylen = 1;
	short collen = 2;
	short vallen = 5;

	int total = keylen + collen + vallen + 3 * sizeof(short) + sizeof(long);
	struct icell *cell = imalloc(consumer, total);

	cell->keylen = keylen;
	cell->collen = collen;
	cell->vallen = vallen;

	randstr((void *) cell + icellheadlen(cell), cell->collen);
	randstr((void *) cell + icellheadlen(cell) + cell->collen, cell->vallen);
	randstr((void *) cell + icellheadlen(cell) + cell->collen + cell->vallen,
			cell->keylen);
	setmvcc(cell, generatemvcc());

	return cell;
}

void setmvcc(struct icell *icell, long mvcc) {

	*(long *) ((void *) icell + icellheadlen(icell) + icell->collen
			+ icell->vallen + icell->keylen) = mvcc;
}

void printicell(struct icell *icell) {

	char colbuf[IZANAMI_MAX_LEN];
	char valbuf[IZANAMI_MAX_LEN];
	char keybuf[IZANAMI_MAX_LEN];
	getcol(icell, colbuf);
	getval(icell, valbuf);
	getkey(icell, keybuf);
	printf("col:%s val:%s key:%s mvcc:%ld\n", colbuf, valbuf, keybuf, getmvcc(icell));
}
