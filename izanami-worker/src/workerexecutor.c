/*
 * workerexecutor.c
 *
 *  Created on: 2018年8月12日
 *      Author: caojx
 */

#include "common.h"
#include "izanami-worker.h"
#include "operation.h"
#include "workerexecutor.h"

#include <stdlib.h>

static char buf[IZANAMI_MAX_LEN] = { '\0' };
static char tablebuf[IZANAMI_MAX_LEN] = { '\0' };
void workerexecute(void *this, int fd) {

	struct worker *_worker = ((struct workerexecutor *) this)->server;
	enum operation op;
	recv(fd, &op, sizeof(op), 0);

	switch (op) {

	case assign: {

		break;
	}
	case put: {

		memset(tablebuf, 0, IZANAMI_MAX_LEN);
		recv(fd, tablebuf, IZANAMI_MAX_LEN, 0);

		memset(buf, 0, IZANAMI_MAX_LEN);
		recv(fd, buf, IZANAMI_MAX_LEN, 0);

		struct icell *icell = (struct icell *) (buf);
		setmvcc(icell, generatemvcc());
		struct iregion *iregion = getiregion(_worker->regionmanager, tablebuf,
				icell);
		if (iregion != NULL)
			inserticell(iregion, icell);
		break;
	}
	case scan: {

		break;
	}
	default:
		printf("no operation type in master");
	}

}

struct workerexecutor *initworkerexecutor() {

	struct workerexecutor *executor = (struct workerexecutor *) malloc(
			sizeof(struct workerexecutor));

	executor->execute = workerexecute;
	return executor;
}
