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

	int ret = 0;

	ret = strcmp(instance1->ip, instance2->ip);
	if (ret == 0) {
		ret = instance1->port - instance2->port;
	}
	return ret;
}

struct mastersideworker *initmastersideworker(configiregioninfoset config,
		char *ip, int port, int fd) {

	struct mastersideworker *instance = (struct mastersideworker *) malloc(
			sizeof(struct mastersideworker));
	instance->set = initiregioninfoset(config);
	strcpy(instance->ip, ip);
	instance->port = port;
	instance->workerfd = fd;
	return instance;
}

struct mastersideworkermanager *initmastersideworkermanager(
		configworkermanager config) {

	struct mastersideworkermanager *manager =
			(struct mastersideworkermanager *) malloc(
					sizeof(struct mastersideworkermanager));
	config(manager);
	return manager;
}


struct mastersideworker *getmastersideworkerbyfd(struct mastersideworkermanager *manager, int fd) {

	struct mastersideworker *ret = NULL;
	int tmp = 0;
	while (tmp < manager->workernum) {
		struct mastersideworker *worker = manager->workers + tmp;
		if (worker->workerfd == fd) {
			ret = worker;
			break;
		}
		tmp ++;
	}
	return ret;
}