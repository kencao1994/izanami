/*
 * iregion.c
 *
 *  Created on: 2018年9月11日
 *      Author: caojx
 */

#include "common.h"
#include "icell.h"
#include "iregion.h"

#include <stdlib.h>


void inserticell(struct iregion *iregion, struct icell *icell) {

	void *buf = imalloc(iregion->consumer, icellsize(icell));
	cpywithlen(buf, (char *)icell, icellsize(icell));
	insertintoskiplist(iregion->memstore, buf, NULL);
}

void flushiregion(struct iregion *iregion, struct ifilemanager *filemanager) {}

int iregioncmp(void *arg1, void *arg2) {

	struct iregion *reg1 = (struct iregion *) arg1;
	struct iregion *reg2 = (struct iregion *) arg2;
	return strcmp(reg1->dirname, reg2->dirname);
}

struct iregionmanager *initiregionmanager(struct iregioninfoset *set) {

	struct iregionmanager *manager = malloc(sizeof(struct iregionmanager));
	manager->maxiregion = set->maxnum;
	manager->iregioncnt = 0;
	manager->iregions = (struct iregion *) malloc(sizeof(struct iregion) * manager->maxiregion
			);

	struct iregioninfo *info = set->infos;
	int index = 0;
	while (index < set->num) {
		struct iregion *reg = manager->iregions + index;
		struct iregioninfo *info = set->infos + index;
		reg->info = info;

		index ++;
	}
	return manager;
}


struct iregion *initiregion(const char *dirname) {

	return NULL;
}
