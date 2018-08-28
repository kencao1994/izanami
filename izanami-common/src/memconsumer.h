/*
 * memconsumer.h
 *
 *  Created on: 2018年8月28日
 *      Author: caojx
 */

#ifndef MEMCONSUMER_H_
#define MEMCONSUMER_H_

#include "block.h"

struct memconsumer {

	void *firstblock;
	void *currentblock;
};

void setconsumerhead(struct consumerblock *block);
struct memconsumer *initmemconsumer();
void *imalloc(struct memconsumer *consumer, int size);

#endif /* MEMCONSUMER_H_ */
