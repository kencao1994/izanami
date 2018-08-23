/*
 * masterexecutor.h
 *
 *  Created on: 2018年8月11日
 *      Author: caojx
 */

#ifndef SRC_MASTEREXECUTOR_H_
#define SRC_MASTEREXECUTOR_H_

#include "izanami-master.h"

struct masterexecutor {

	void (*execute)(void *this, int fd);
	struct master *server;
};

struct masterexecutor *initmasterexecutor();

#endif /* SRC_MASTEREXECUTOR_H_ */
