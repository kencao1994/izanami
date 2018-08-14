/*
 * ianami-worker.c
 *
 *  Created on: 2018年8月12日
 *      Author: caojx
 */

#include "common.h"
#include "config.h"
#include "dictionary.h"
#include "iniparser.h"
#include "iregioninfo.h"
#include "izanami-worker.h"
#include "workerexecutor.h"

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

void configworker(struct networkserver *server) {

	dictionary *dict = getdict();
	server->port = iniparser_getint(dict, IZANAMI_WORKER_ETH, 7000);
	server->eth = iniparser_getstring(dict, IZANAMI_WORKER_PORT, "0.0.0.0");
	server->maxconn = iniparser_getint(dict, IZANAMI_WORKER_MAXCONN, 1024);
	server->executor = (struct executor*) initworkerexecutor();
}

void *initreportthread(void *args) {

	struct workerexecutor *_worker = (struct workerexecutor *) args;
	dictionary *dict = getdict();
	int period = iniparser_getint(dict, IZANAMI_WORKER_REPORT_PERIOD, 10);

	while (TRUE) {

		setiregioninfoset(_worker->server->set, _worker->server->datadir);
		sleep(period);
	}

	return NULL;
}

void configworkerset(struct iregioninfoset* set) {

	dictionary *dict = getdict();
	set->maxnum = iniparser_getint(dict, IZANAMI_WORKER_REGMAXNUM, 1024);
	set->num = 0;
}

struct worker *initworker() {

	struct worker *_worker = (struct worker *) malloc(sizeof(struct worker));
	_worker->networkserver = initnetworkserver(configworker);
	_worker->set = initiregioninfoset(configworkerset);
	struct workerexecutor *executor =
			(struct workerexecutor *) _worker->networkserver->executor;
	executor->server = _worker;

	// 初始化与izanami-master通信的线程
	pthread_create(&(_worker->reportthread), NULL, initreportthread, _worker);

	return _worker;
}

void waitworkerfinish(struct worker *_worker) {

	printf("waiting worker to finish\n");
	pthread_join(_worker->networkserver->serverthread, NULL);
}
