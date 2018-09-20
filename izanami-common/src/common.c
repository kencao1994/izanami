/*
 * common.c
 *
 *  Created on: 2018年8月11日
 *      Author: caojx
 */

#include "common.h"
#include "config.h"
#include "iniparser.h"

static dictionary *dict = NULL;

dictionary *getdict() {

	if (dict == NULL) {
		dict = iniparser_load(IZANAMI_CONFIG);
	}

	return dict;
}

void readline(int fd, char *buf, int buflen) {

	char *ptr = buf;
	memset(buf, 0, buflen);
	int readcount = read(fd, buf, buflen);
	if (readcount == 0) {
		printf("no lines left\n");
		return;
	}
	int seekindex = 0;
	while (*ptr != NEXTLINE) {
		seekindex++;
		ptr++;
	}
	buf[seekindex] = '\0';
	lseek(fd, seekindex - readcount + 1, SEEK_CUR);
}

static int ready = FALSE;

void setready() {

	ready = TRUE;
}

int getready() {

	return ready;
}

int halffind(void *arr, int size, int left, int right, void *target,
		int (*cmp)(void *arg1, void *arg2)) {

	if (left == right) {
		if (cmp(arr + left * size, target)) {
			return -1;
		} else {
			return left;
		}
	} else if (left > right) {
		return -1;
	}

	int mid = (left + right) / 2;
	int ret = cmp(arr + mid * size, target);
	if (ret > 0) {
		return halffind(arr, size, left, mid - 1, target, cmp);
	} else if (ret < 0) {
		return halffind(arr, size, mid + 1, right, target, cmp);
	} else {
		return mid;
	}
}

int getfileblocksize() {

	dictionary *dict = getdict();
	return iniparser_getint(dict, IZANAMI_FILE_BLOCKSIZE, 1024 * 1024);
}

int getfileblockmetasize() {

	dictionary *dict = getdict();
	return iniparser_getint(dict, IZANAMI_FILE_BLOCKMETASIZE, 4 * 1024);
}

void cpywithlen(char *to, char *from, int len) {

	int i = 0;
	while (i < len) {
		to[i] = from[i];
		i++;
	}
}
