/*
 * mastersideworker.h
 *
 *  Created on: 2018年8月16日
 *      Author: caojx
 */

#ifndef MASTERSIDEWORKER_H_
#define MASTERSIDEWORKER_H_

#include "config.h"
#include "iregioninfo.h"

struct mastersideworker {

	char ip[IZANAMI_MAX_IP_LEN];
	int port;
	int workerfd;
	struct iregioninfoset *set;
};

struct mastersideworkermanager{

	int maxworkernum;
	int workernum;
	struct mastersideworker *workers;
};

int mastersideworkercmp(void *arg1, void *arg2);

struct mastersideworker *initmastersideworker(configiregioninfoset config, char *ip, int port, int fd);

typedef void (*configworkermanager)(struct mastersideworkermanager *manager);
struct mastersideworkermanager *initmastersideworkermanager(configworkermanager config);
struct mastersideworker *getmastersideworkerbyfd(struct mastersideworkermanager *manager, int fd);
#endif /* MASTERSIDEWORKER_H_ */