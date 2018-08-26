/*
 * mastersideworker.c
 *
 *  Created on: 2018年8月16日
 *      Author: caojx
 */

#include "mastersideworker.h"

#include <stdlib.h>
#include <string.h>

int mastersideworkercmp(void *arg1, void *arg2) {

	struct mastersideworker *instance1 = (struct mastersideworker *) arg1;
	struct mastersideworker *instance2 = (struct mastersideworker *) arg2;

	int ret = strcmp(instance1->ip, instance2->ip);

	return ret;
}

struct mastersideworkermanager *initmastersideworkermanager(
		configworkermanager config) {

	struct mastersideworkermanager *manager =
			(struct mastersideworkermanager *) malloc(
					sizeof(struct mastersideworkermanager));
	config(manager);
	return manager;
}

struct mastersideworker *getmastersideworkerbyfd(
		struct mastersideworkermanager *manager, int fd) {

	struct mastersideworker *ret = NULL;
	int tmp = 0;
	while (tmp < manager->workernum) {
		struct mastersideworker *worker = manager->workers + tmp;
		if (worker->workerfd == fd) {
			ret = worker;
			break;
		}
		tmp++;
	}
	return ret;
}

void addmastersideworker(struct mastersideworkermanager *manager, int fd,
		char *ip) {

	struct mastersideworker *worker = manager->workers + manager->workernum;
	manager->workernum++;
	worker->workerfd = fd;
	strcpy(worker->ip, ip);
}

void getregioninfos(struct mastersideworkermanager *manager, int fd) {

	int count = manager->workernum;
	struct mastersideworker *worker = (struct mastersideworker *) malloc(
			sizeof(struct mastersideworker) * count);

	int i = 0;
	for (; i < count; i++) {

		struct mastersideworker *tmp1 = worker + i;
		struct mastersideworker *tmp2 = manager->workers + i;

		strcpy(tmp1->ip, tmp2->ip);
		tmp1->set = tmp2->set;
		tmp1->workerfd = -1;
	}

	send(fd, &count, sizeof(int), 0);
	send(fd, worker, sizeof(struct mastersideworker) * count, 0);
	/**
	 * 规整化内存，暂时不用内存池
	 */
	free(worker);
}
