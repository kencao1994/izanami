/*
 * masterexecutor.c
 *
 *  Created on: 2018年8月11日
 *      Author: caojx
 */

#include "masterexecutor.h"

#include <stdlib.h>

void masterexecute(void *this, int fd) {}

struct masterexecutor *initmasterexecutor() {

	struct masterexecutor * executor = (struct masterexecutor*) malloc(
			sizeof(struct masterexecutor));

	executor->execute = masterexecute;
	return executor;
}
