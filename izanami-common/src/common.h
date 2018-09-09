/*
 * common.h
 *
 *  Created on: 2018年8月11日
 *      Author: caojx
 */

#ifndef SRC_COMMON_H_
#define SRC_COMMON_H_

#include "dictionary.h"

#include <stdlib.h>

#define NEXTLINE '\n'

#define TRUE 1
#define FALSE 0

struct networkserver;
struct executor;
struct server;

dictionary *getdict();
void readline(int fd, char *buf, int buflen);
void setready();
int getready();
int halffind(void *arr, int size, int left, int right, void *target,
		int (*cmp)(void *arg1, void *arg2));
int getfileblocksize();
int getfileblockmetasize();

#endif /* SRC_COMMON_H_ */
