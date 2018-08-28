/*
 * mempool.c
 *
 *  Created on: 2018年8月26日
 *      Author: caojx
 */

#include "config.h"
#include "common.h"
#include "mempool.h"
#include "skiplist.h"
#include "workercommon.h"

#include <stdlib.h>

static struct mempool *pool = NULL;
static int blocksize = 0;
static int metasize = 0;
static int datasize = 0;

int blockinfocmp(void *arg1, void *arg2) {

	struct blockinfo *info1 = (struct blockinfo *) arg1;
	struct blockinfo *info2 = (struct blockinfo *) arg2;
	return info1->ptr - info2->ptr;
}

struct memconsumer *getemptyconsumer(void *block) {

	struct consumerhead *head = (struct consumerhead *)block;
	setconsumerhead(head);

	struct memconsumer *tmp = (struct memconsumer *) malloc(sizeof(struct memconsumer));
	tmp->currentblock = tmp->firstblock = block;
	return tmp;
}

struct mempool *getmempool() {

	if (pool == NULL) {
		dictionary *dict = getdict();
		long poolsize = iniparser_getlongint(dict, IZANAMI_MEMPOOL_MAXSIZE,
				1204 * 1024 * 1024);
		blocksize = iniparser_getint(dict, IZANAMI_MEMPOOL_BLOCKSIZE,
				1024 * 64);
		metasize = iniparser_getint(dict, IZANAMI_MEMPOOL_METASIZE, 1024 * 4);
		datasize = blocksize - metasize;

		pool = (struct mempool *) malloc(sizeof(struct mempool));
		pool->start = malloc(sizeof(poolsize));
		pool->freelist = initskiplist(blockinfocmp);
		pool->usedlist = initskiplist(blockinfocmp);

		struct memconsumer *freeconsumer = getemptyconsumer(pool->start);
		struct memconsumer *usedconsumer = getemptyconsumer(pool->start +  blocksize);

		setconsumer(pool->freelist, freeconsumer);
		setconsumer(pool->usedlist, usedconsumer);

		int count = poolsize / blocksize;
		int i = 2;
		for (; i < count; i++) {
			insertintoskiplist(pool->freelist, pool->start + i * blocksize);
		}

	}

	return pool;
}

struct leafinode *getfirst(struct skiplist *list) {

	struct leafinode *inode = NULL;

	struct steaminode *head = list->routenode;
	while (head->down != NULL) {
		head = head->down;
	}

	inode = (struct leafinode *) head->post;

	return inode;
}

void insertinodeintoskiplist(struct skiplist *list, struct leafinode *inode) {

	struct leafinode *prenode = findfromskiplist(list, inode->element);
	inode->post = prenode->post;
	inode->pre = prenode;
	prenode->post->pre = inode;
	prenode->post = inode;
}

struct blockinfo *getblock(struct mempool *pool) {

	struct leafinode *inode = getfirst(pool->freelist);
	deletefromskiplist(pool->freelist, inode->element);
	insertinodeintoskiplist(pool->usedlist, inode);
	return (struct blockinfo *) inode->element;
}

void returnblock(struct mempool *pool, struct blockinfo *info) {

	struct leafinode *inode = deletefromskiplist(pool->usedlist, info);
	insertinodeintoskiplist(pool->freelist, inode);
}

void setconsumerhead(struct consumerhead *head) {

	head->dataused = 0;
	head->next = NULL;
}

struct memconsumer *initmemconsumer() {

	struct memconsumer *consumer = (struct memconsumer *) malloc(
			sizeof(struct memconsumer));
	consumer->firstblock = consumer->currentblock = getblock(getmempool());

	struct consumerhead *head = consumer->firstblock;
	setconsumerhead(head);
	return consumer;
}

void *imalloc(struct memconsumer *consumer, int size) {

	struct consumerhead *head = consumer->currentblock;
	int beforeallocate;

	if (head->dataused + size < datasize) {
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

	return head + metasize + beforeallocate;
}

