/*
 * mempool.h
 *
 *  Created on: 2018年8月26日
 *      Author: caojx
 */

#ifndef MEMPOOL_H_
#define MEMPOOL_H_

#include "workercommon.h"

struct blockinfo {

	void *ptr;
};

struct mempool {

	void *start;
	struct skiplist *freelist;
	struct skiplist *usedlist;
};

struct mempool *getmempool();
struct blockinfo *getblock(struct mempool *pool);
void returnblock(struct mempool *pool, struct blockinfo *info);

struct memconsumer {

	void *firstblock;
	void *currentblock;
};


struct consumerhead {

	int dataused;
	struct blockinfo *next;
};

void setconsumerhead(struct consumerhead *head);
struct memconsumer *initmemconsumer();
void *imalloc(struct memconsumer *consumer, int size);

#endif /* MEMPOOL_H_ */
