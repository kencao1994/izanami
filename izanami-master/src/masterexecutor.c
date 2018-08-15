/*
 * masterexecutor.c
 *
 *  Created on: 2018年8月11日
 *      Author: caojx
 */

#include "masterexecutor.h"
#include "operation.h"

#include <stdlib.h>

void masterexecute(void *this, int fd) {

	struct master *_master;
	enum operation op;
	recv(fd, &op, sizeof(op), 0);

	switch (op) {

	case report:
		printf("executing report");
		break;
	case assign:
		break;
	default:;
	}

}

struct masterexecutor *initmasterexecutor() {

	struct masterexecutor * executor = (struct masterexecutor*) malloc(
			sizeof(struct masterexecutor));

	executor->execute = masterexecute;
	return executor;
}
