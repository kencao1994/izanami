/*
 * memconsumer.c
 *
 *  Created on: 2018年8月28日
 *      Author: caojx
 */

#include "block.h"
#include "memconsumer.h"
#include "mempool.h"

#include <stdlib.h>

void setconsumerhead(struct consumerblock *head) {

	memset(head, 0, getblocksize());

	head->dataused = 0;
	head->next = NULL;
}

struct memconsumer *initmemconsumer() {

	struct memconsumer *consumer = (struct memconsumer *) malloc(
			sizeof(struct memconsumer));
	consumer->firstblock = consumer->currentblock = getblock(getmempool());

	struct consumerblock *head = consumer->firstblock;
	setconsumerhead(head);
	return consumer;
}

void *imalloc(struct memconsumer *consumer, int size) {

	struct consumerblock *head = consumer->currentblock;
	int beforeallocate;

	if (head->dataused + size < getdatasize()) {
		beforeallocate = head->dataused;
		head->dataused += size;
	} else {
		beforeallocate = 0;
		struct blockinfo *info = getblock(getmempool());
		head->next = info;
		head = info;
		setconsumerhead(head);
		head->dataused += size;
	}

	return (void *)((long)consumer->currentblock + getmetasize() + beforeallocate);
}