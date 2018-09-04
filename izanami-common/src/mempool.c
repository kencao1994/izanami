/*
 * mempool.c
 *
 *  Created on: 2018年8月26日
 *      Author: caojx
 */

#include "config.h"
#include "common.h"
#include "iniparser.h"
#include "memconsumer.h"
#include "mempool.h"
#include "skiplist.h"
#include "workercommon.h"

#include <stdlib.h>

static struct mempool *pool = NULL;
static int blocksize = 0;
static int metasize = 0;
static int datasize = 0;

int getblocksize() {

	return blocksize;
}

int getmetasize() {

	return metasize;
}

int getdatasize() {

	return datasize;
}

int blockinfocmp(void *arg1, void *arg2) {

	if (arg1 == arg2) {
		return 0;
	}
	int ret = (arg1 > arg2) ? 1 : -1;
	return ret;
}

struct memconsumer *getemptyconsumer(void *block) {

	struct consumerblock *head = (struct consumerblock *) block;
	setconsumerhead(head);

	struct memconsumer *tmp = (struct memconsumer *) malloc(
			sizeof(struct memconsumer));
	tmp->currentblock = tmp->firstblock = block;
	return tmp;
}

struct mempool *getmempool() {

	if (pool == NULL) {
		dictionary *dict = getdict();
		int poolsize = iniparser_getint(dict, IZANAMI_MEMPOOL_MAXSIZE,
				1024 * 1024 * 1024);
		blocksize = iniparser_getint(dict, IZANAMI_MEMPOOL_BLOCKSIZE,
				64 * 1024);
		metasize = iniparser_getint(dict, IZANAMI_MEMPOOL_METASIZE, 1024 * 4);
		datasize = blocksize - metasize;

		pool = (struct mempool *) malloc(sizeof(struct mempool));
		pool->start = malloc(poolsize);
		pool->freelist = initskiplist(blockinfocmp);
		pool->usedlist = initskiplist(blockinfocmp);

		struct memconsumer *freeconsumer = getemptyconsumer(pool->start);
		setconsumer(pool->freelist, freeconsumer);

		struct memconsumer *usedconsumer = getemptyconsumer(
				pool->start + blocksize);
		setconsumer(pool->usedlist, usedconsumer);

		int count = poolsize / blocksize;
		int i = 2;
		for (; i < count; i++) {
			insertintoskiplist(pool->freelist, pool->start + i * blocksize);
			void *start = pool->start + i * blocksize;
			setconsumerhead(start);
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

	struct steaminode *upperinode = findfromskiplistbylevel(list,
			inode->element, list->layer - 1);
	struct leafinode *prenode = NULL;
	int step = 0;
	if (upperinode == NULL) {
		prenode = findfromskiplist(list, inode->element);
	} else {
		step = findfromsubskiplist(list, upperinode->down, inode->element,
				&prenode);
	}
	list->count++;
	insertbefore(prenode, inode);
	if (upperinode != NULL && step == 0) {
		upperinode->down = inode;
	}
	//	inode->post = prenode->post;
//	inode->pre = prenode;
//	prenode->post->pre = inode;
//	prenode->post = inode;
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
