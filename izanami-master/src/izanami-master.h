/*
 * izanami-master.h
 *
 *  Created on: 2018年8月11日
 *      Author: caojx
 */

#ifndef SRC_IZANAMI_MASTER_H_
#define SRC_IZANAMI_MASTER_H_

#include "networkserver.h"

struct master {

	struct networkserver *networkserver;
};

struct master * initmaster();
void waitmasterfinish(struct master *_master);

#endif /* SRC_IZANAMI_MASTER_H_ */
