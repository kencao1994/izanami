/*
 * tablemanager.h
 *
 *  Created on: 2018年8月26日
 *      Author: caojx
 */

#ifndef SRC_TABLEMANAGER_H_
#define SRC_TABLEMANAGER_H_

#include "config.h"

struct tablename {

	char name[IZANAMI_MAX_TAB_LEN];
};

void settablename(struct tablename *tablename, const char *name);
int tablenamecmp(void *arg1, void *arg2);

struct tablemanager {

	struct tablename *tables;
	int tablenum;
	int maxtablenum;
};

struct tablemanager *inittablemanager();
int exists(struct tablemanager *manager, struct tablename *table);
void loadtablename(struct tablemanager *manager);
void savetablename(struct tablemanager *manager);
void inserttable(struct tablemanager *manager, char *table);

#endif /* SRC_TABLEMANAGER_H_ */
