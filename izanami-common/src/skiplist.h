/*
 * skiplist.h
 *
 *  Created on: 2018年8月26日
 *      Author: caojx
 */

#ifndef SKIPLIST_H_
#define SKIPLIST_H_

#include "workercommon.h"

struct steaminode {

	void *element;

	struct steaminode *pre;
	struct steaminode *post;
	struct steaminode *down;
};
void setsteaminode(struct steaminode *inode);
typedef int (*elementcmp)(void *arg1, void* arg2);

struct leafinode {

	void *element;

	struct leafinode *pre;
	struct leafinode *post;
};

struct skiplist {

	int count;
	int layer;
	int repeatable;

	struct steaminode *routenode;
	struct memconsumer *consumer;
	elementcmp cmp;
};

struct skiplist *initskiplist(elementcmp cmp);

void setconsumer(struct skiplist *list, struct memconsumer *consumer);
void setrepeatable(struct skiplist *list);
/**
 * 从skiplist中删除element
 * 返回element对应的leafnode
 * 返回NULL表示没有element
 */
struct leafinode *deletefromskiplist(struct skiplist *list, void *element);
/**
 * 返回刚好这个或者前面一个元素
 */
struct leafinode *findfromskiplist(struct skiplist *list, void *element);

struct leafinode *findfromskiplistbylevel(struct skiplist *list, void *element,
		int level);
/**
 * 返回指向插入inode的指针
 */
struct leafinode *insertintoskiplist(struct skiplist *list, void *element,
		struct leafinode *targetnode);

void insertbefore(void *arg1, void *arg2);

void tolocalfile(struct skiplist *list, int fd);

void destroyskiplist(struct skiplist *list);

#endif /* SKIPLIST_H_ */
