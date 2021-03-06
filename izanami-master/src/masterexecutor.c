/*
 * masterexecutor.c
 *
 *  Created on: 2018年8月11日
 *      Author: caojx
 */

#include "config.h"
#include "izanami-master.h"
#include "masterexecutor.h"
#include "masteriregioninfo.h"
#include "mastersideworker.h"
#include "operation.h"

#include <stdlib.h>

static void *tabnamebuf[IZANAMI_MAX_TAB_LEN];
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
	case route: {
		getregioninfos(_master->workermanager, fd);
		break;
	}
	case create: {
		recv(fd, tabnamebuf, IZANAMI_MAX_TAB_LEN, 0);
		int splits = 0;
		recv(fd, &splits, sizeof(int), 0);
		char *keybuf = (char *) malloc(IZANAMI_MAX_KEY_LEN * splits);

		docreatetable(_master, tabnamebuf, NULL);
		free(keybuf);
		break;
	}
	default:
		printf("no operation type in master");
	}

}

struct masterexecutor *initmasterexecutor() {

	struct masterexecutor * executor = (struct masterexecutor*) malloc(
			sizeof(struct masterexecutor));

	executor->execute = masterexecute;
	return executor;
}
