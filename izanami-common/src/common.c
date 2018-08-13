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

	memset(buf, 0, buflen);
	int readcount = read(fd, buf, buflen);
	if (readcount == 0) {
		printf("no lines left\n");
		exit(-1);
	}
	char *ptr = buf;
	int seekindex = 0;
	while (*ptr != NEXTLINE) {
		seekindex++;
		ptr++;
	}
	buf[seekindex] = '\0';
	printf("%d %d", seekindex, readcount);
	lseek(fd, seekindex - readcount + 1, SEEK_CUR);
}
