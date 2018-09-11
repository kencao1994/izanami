/*
 * izanami-worker.h
 *
 *  Created on: 2018年8月12日
 *      Author: caojx
 */

#ifndef IZANAMI_WORKER_H_
#define IZANAMI_WORKER_H_

#include "ifile.h"
#include "networkserver.h"

struct worker {

	struct networkserver *networkserver;
	struct iregioninfoset *set;
	struct ifilemanager *filemanager;
	char *datadir;
	pthread_t reportthread;
};

struct worker *initworker();
void waitworkerfinish(struct worker *_worker);
struct ifilemanager *getifilemanager();

#endif /* IZANAMI_WORKER_H_ */
