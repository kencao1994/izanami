/*
 * izanami-worker.h
 *
 *  Created on: 2018年8月12日
 *      Author: caojx
 */

#ifndef IZANAMI_WORKER_H_
#define IZANAMI_WORKER_H_

#include "ifile.h"
#include "iregion.h"
#include "networkserver.h"

struct worker {

	struct networkserver *networkserver;
	struct iregioninfoset *set;
	struct ifilemanager *filemanager;
	struct iregionmanager *regionmanager;
	char *datadir;
	pthread_t reportthread;
};

struct worker *initworker();
void waitworkerfinish(struct worker *_worker);
struct ifilemanager *getifilemanager();
struct iregionmanager *getiregionmanager();

#endif /* IZANAMI_WORKER_H_ */
