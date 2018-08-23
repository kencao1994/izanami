/*
 * workerexecutor.h
 *
 *  Created on: 2018年8月12日
 *      Author: caojx
 */

#ifndef WORKEREXECUTOR_H_
#define WORKEREXECUTOR_H_

#include "izanami-worker.h"

struct workerexecutor {

	void (*execute)(void *this, int fd);
	struct worker *server;
};

struct workerexecutor *initworkerexecutor();

#endif /* WORKEREXECUTOR_H_ */
