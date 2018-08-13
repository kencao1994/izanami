/*
 * izanami-worker.h
 *
 *  Created on: 2018年8月12日
 *      Author: caojx
 */

#ifndef IZANAMI_WORKER_H_
#define IZANAMI_WORKER_H_

#include "networkserver.h"

struct worker {

	struct networkserver *networkserver;
	pthread_t reportthread;
};

struct worker * initworker();
void waitworkerfinish(struct worker *_worker);

#endif /* IZANAMI_WORKER_H_ */
