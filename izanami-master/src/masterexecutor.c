/*
 * masterexecutor.c
 *
 *  Created on: 2018年8月11日
 *      Author: caojx
 */

#include "masterexecutor.h"
#include "masteriregioninfo.h"
#include "mastersideworker.h"
#include "operation.h"

#include <stdlib.h>

static void *regbuf[sizeof(struct iregioninfo) * IZANAMI_WOEKER_REGMAXNUM_MAX];

void masterexecute(void *this, int fd) {

	struct master *_master = ((struct masterexecutor *) this)->server;
	enum operation op;
	recv(fd, &op, sizeof(op), 0);

	switch (op) {

	case report: {
		int num = 0;
		recv(fd, &num, sizeof(int), 0);
		recv(fd, regbuf, sizeof(struct iregioninfo) * num, 0);

		struct mastersideworker *worker = getmastersideworkerbyfd(
				_master->workermanager, fd);
		recviregionfromworker(worker->set, num, regbuf);
		break;
	}
	case assign: {
		break;
	}
	default:
		;
	}

}

struct masterexecutor *initmasterexecutor() {

	struct masterexecutor * executor = (struct masterexecutor*) malloc(
			sizeof(struct masterexecutor));

	executor->execute = masterexecute;
	return executor;
}
