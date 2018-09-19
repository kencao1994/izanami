/*
 * iregion.c
 *
 *  Created on: 2018年9月11日
 *      Author: caojx
 */

#include "common.h"
#include "config.h"
#include "icell.h"
#include "iregion.h"
#include "iregioninfo.h"

#include <fcntl.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>

void inserticell(struct iregion *iregion, struct icell *icell) {

	void *buf = imalloc(iregion->consumer, icellsize(icell));
	cpywithlen(buf, (char *) icell, icellsize(icell));
	insertintoskiplist(iregion->memstore, buf, NULL);
}

static char filename[IZANAMI_MAX_LEN];
void flushiregion(struct iregion *iregion, struct ifilemanager *filemanager) {

	memset(filename, '\0', IZANAMI_MAX_LEN);
	strcpy(filename, getregiondir(iregion->info));
	int len = strlen(filename);
	cpywithlen(filename + len , IZANAMI_DELIMITER, strlen(IZANAMI_DELIMITER));
	len = strlen(filename);

	int index = 0;
	while (index < IZANAMI_FILE_LEN) {

		filename[len + index] = '0' + rand() % 10;
	}

	int fd = open(filename, O_CREAT | O_RDWR, S_IRWXU);

	struct skiplist *tmp = iregion->memstore;

	iregion->consumer = initmemconsumer();
	iregion->memstore = initskiplist(icellcmp);
	iregion->memstore->consumer = iregion->consumer;
	iregion->memstore->routenode = imalloc(iregion->consumer,
			sizeof(struct steaminode));
	setsteaminode(iregion->memstore->routenode);

	tolocalfile(tmp, fd);
	destroyskiplist(tmp);
	//增加新的ifile
	initifile(filemanager, filename);

	close(fd);
}

int iregioncmp(void *arg1, void *arg2) {

	struct iregioninfo *reg1 = ((struct iregion *) arg1)->info;
	struct iregioninfo *reg2 = ((struct iregion *) arg2)->info;
	return regioncmp(reg1, reg2);
}

void loadiregion(struct iregion *iregion, struct iregioninfo *info) {

	iregion->consumer = initmemconsumer();
	iregion->info = info;
	strcpy(iregion->dirname, getregiondir(info));
	iregion->memstore = initskiplist(icellcmp);
	iregion->memstore->consumer = iregion->consumer;
	iregion->memstore->routenode = imalloc(iregion->consumer,
			sizeof(struct steaminode));
	setsteaminode(iregion->memstore->routenode);

}

struct iregionmanager *initiregionmanager(struct iregioninfoset *set) {

	struct iregionmanager *manager = malloc(sizeof(struct iregionmanager));
	manager->maxiregion = set->maxnum;
	manager->iregioncnt = 0;
	manager->iregions = (struct iregion *) malloc(
			sizeof(struct iregion) * manager->maxiregion);

	struct iregioninfo *info = set->infos;
	int index = 0;
	while (index < set->num) {

		loadiregion(manager->iregions + index, info + index);
		index++;
	}
	return manager;
}

