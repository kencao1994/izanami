/*
 * workerexecutor.c
 *
 *  Created on: 2018年8月12日
 *      Author: caojx
 */

#include "common.h"
#include "izanami-worker.h"
#include "operation.h"
#include "readaction.h"
#include "workerexecutor.h"

#include <stdlib.h>

static char buf[IZANAMI_MAX_LEN] = { '\0' };
static char tablebuf[IZANAMI_MAX_LEN] = { '\0' };
static char startkeybuf[IZANAMI_MAX_LEN] = {'\0'};
static char endkeybuf[IZANAMI_MAX_LEN] = {'\0'};
void workerexecute(void *this, int fd) {

	struct worker *_worker = ((struct workerexecutor *) this)->server;
	enum operation op;
	recv(fd, &op, sizeof(op), 0);

	switch (op) {

	case assign: {
		printf("nothing \n");
		break;
	}
	case put: {

		memset(tablebuf, 0, IZANAMI_MAX_LEN);
		recv(fd, tablebuf, IZANAMI_MAX_LEN, 0);

		memset(buf, 0, IZANAMI_MAX_LEN);
		recv(fd, buf, IZANAMI_MAX_LEN, 0);

		struct icell *icell = (struct icell *) (buf);
		struct iregion *iregion = getiregion(_worker->regionmanager, tablebuf,
				icell);
		if (iregion != NULL) {
			setmvcc(icell, generatemvcc());
			inserticell(iregion, icell);
		}
		break;
	}
	case scan: {

		memset(tablebuf, 0, IZANAMI_MAX_LEN);
		recv(fd, tablebuf, IZANAMI_MAX_LEN, 0);

		memset(startkeybuf, 0, IZANAMI_MAX_LEN);
		recv(fd, startkeybuf, IZANAMI_MAX_LEN, 0);

		memset(endkeybuf, 0, IZANAMI_MAX_LEN);
		recv(fd, endkeybuf, IZANAMI_MAX_LEN, 0);

		struct iregion *start = getiregionbyregioninfo(_worker->regionmanager, tablebuf, startkeybuf, endkeybuf);

		if (start != NULL){
			struct readaction *action = initreadaction(start, fd);
			doread(action);
			cleanaction(action);
		}

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
