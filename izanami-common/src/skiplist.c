/*
 * skiplist.c
 *
 *  Created on: 2018年8月26日
 *      Author: caojx
 */

#include "common.h"
#include "config.h"
#include "dictionary.h"
#include "iniparser.h"
#include "memconsumer.h"
#include "mempool.h"
#include "skiplist.h"

#include <stdlib.h>

static int initialized = FALSE;
static int maxskip = 0;
static int maxlayer = 0;

void setleafinode(struct leafinode *inode) {

	inode->element = NULL;
	inode->post = inode->pre = NULL;
}

void setsteaminode(struct steaminode *inode) {

	inode->element = NULL;
	inode->down = inode->post = inode->pre = NULL;
}

struct skiplist *initskiplist(elementcmp cmp) {

	if (!initialized) {

		dictionary *dict = getdict();
		maxskip = iniparser_getint(dict, IZANAMI_SKIPLIST_MAXSKIP, 64);
		maxlayer = iniparser_getint(dict, IZANAMI_SKIPLIST_MAXLAYER, 8);
		initialized = TRUE;
	}

	struct skiplist *list = (struct skiplist *) malloc(sizeof(struct skiplist));
	list->count = 0;
	list->layer = 1;
	list->repeatable = FALSE;
	list->cmp = cmp;
//	list->consumer = initmemconsumer();
//	list->routenode = imalloc(list->consumer, sizeof(struct steaminode));
//	setsteaminode(list->routenode);
	return list;
}

void setrepeatable(struct skiplist *list) {

	list->repeatable = TRUE;
}

void deletefromsublist(struct skiplist *list, struct steaminode *uppernode,
		struct leafinode *start, void *element, int level,
		struct leafinode **result) {

	struct leafinode *prenode = NULL;
	findfromsubskiplist(list, start, element, &prenode);
	if (list->layer == level) {

		if (prenode != NULL && list->cmp(prenode->element, element) == 0) {

			prenode->post->pre = prenode->pre;
			prenode->pre->post = prenode->post;
			if (uppernode != NULL && uppernode->element != NULL) {
				uppernode->down = prenode->post;
			}
			*result = prenode;
		}
	} else {

		deletefromsublist(list, prenode, ((struct steaminode *) prenode)->down,
				element, level + 1, result);
	}

}

struct leafinode *deletefromskiplist(struct skiplist *list, void *element) {

	struct leafinode *ret = NULL;

	deletefromsublist(list, NULL, list->routenode, element, 1, &ret);

	if (ret != NULL) {
		list->count--;
	}

	return ret;
}

int findfromsubskiplist(struct skiplist *list, struct steaminode *start,
		void *element, struct leafinode **result) {

	int step = 0;
	struct leafinode *tmp = (struct leafinode *) start;
	struct leafinode *pre = tmp;
	//该层没有元素 返回第一个头元素
	if (tmp->element == NULL && tmp->post == NULL) {

		*result = tmp;
		return 1;
	}

	while (tmp != NULL && list->cmp(tmp->element, element) < 0) {
		step++;
		pre = tmp;
		tmp = tmp->post;
	}

	if (step == 0 && list->cmp(tmp->element, element) == 0) {
		//等于最小
		*result = pre;
	} else if (step == 0) {
		//小于最小
		*result = pre->pre;
	}
	if (tmp == NULL) {
		//大于最大
		*result = pre;
	} else if (list->cmp(tmp->element, element) == 0) {
		//刚好匹配
		*result = tmp;
	} else {
		//没有正好匹配，也不是最小或者最大返回前一个
		*result = tmp->pre;
	}

	return step;
}

struct leafinode *findfromskiplist(struct skiplist *list, void *element) {

	return (struct leafinode *) findfromskiplistbylevel(list, element,
			list->layer);
}

struct leafinode *findfromskiplistbylevel(struct skiplist *list, void *element,
		int level) {

	int tmplevel = 0;
	struct steaminode *start = list->routenode;
	struct steaminode *tmp = NULL;
	while (tmplevel != level && tmplevel < list->layer) {
		findfromsubskiplist(list, start, element, (struct leafinode **) &tmp);
		start = tmp->down;
		tmplevel++;
	}

	return (struct leafinode *) tmp;
}

void insertbefore(void *arg1, void *arg2) {

	struct leafinode *target = (struct leafinode *) arg1;
	struct leafinode *element = (struct leafinode *) arg2;

	element->post = target->post;
	element->pre = target;

	if (target->post != NULL) {
		target->post->pre = element;
	}
	target->post = element;
}

int insertintosubskiplist(struct skiplist *list, struct steaminode *start,
		void *element, struct leafinode **result, int level,
		struct leafinode **closestnode) {

	struct leafinode *nodebefore = NULL;
	int step = findfromsubskiplist(list, start, element, &nodebefore);
	*closestnode = nodebefore;

	if (level == list->layer) {
		int ret = list->cmp(nodebefore->element, element);
		if (ret != 0 || list->repeatable) {
			struct leafinode *inode = imalloc(list->consumer,
					sizeof(struct leafinode));

			setleafinode(inode);
			insertbefore(nodebefore, inode);
			inode->element = element;
			*result = inode;
			*closestnode = inode;
		}
	} else {

		struct leafinode *node = NULL;
		int ret = insertintosubskiplist(list,
				((struct steaminode *) nodebefore)->down, element, result,
				level + 1, &node);

		if (ret > maxskip && result != NULL) {
			struct steaminode *newinode = imalloc(list->consumer,
					sizeof(struct steaminode));
			setsteaminode(newinode);

			insertbefore((struct leafinode *) nodebefore,
					(struct leafinode *) newinode);

			newinode->element = element;
			newinode->down = node;
			*closestnode = newinode;
		}
	}

	return step;
}

struct leafinode *insertintoskiplist(struct skiplist *list, void *element) {

	struct leafinode *ret = NULL;
	struct leafinode *closest = NULL;
	int step = insertintosubskiplist(list, list->routenode, element, &ret, 1,
			&closest);

	if (ret != NULL) {
		list->count++;
	}

	if (step > maxskip && list->layer < maxlayer) {
		list->layer++;

		struct steaminode *newroutenode = imalloc(list->consumer,
				sizeof(struct steaminode));
		setsteaminode(newroutenode);

		struct steaminode *firstnode = imalloc(list->consumer,
				sizeof(struct steaminode));
		setsteaminode(firstnode);
		firstnode->element = element;
		firstnode->down = closest;

		insertbefore(newroutenode, firstnode);

		newroutenode->down = list->routenode;
		list->routenode = newroutenode;
	}

	return ret;
}

void setconsumer(struct skiplist *list, struct memconsumer *consumer) {

	list->consumer = consumer;
	list->routenode = imalloc(list->consumer, sizeof(struct steaminode));
//	printf("start route is %p\n", list->routenode);
//	printf("%p:%p:%p:%p", list->routenode->down, list->routenode->element, list->routenode->post, list->routenode->pre);
	setsteaminode(list->routenode);
}
