/*
 * ianami-worker.c
 *
 *  Created on: 2018年8月12日
 *      Author: caojx
 */

#include "clienttype.h"
#include "common.h"
#include "config.h"
#include "dictionary.h"
#include "iniparser.h"
#include "iregioninfo.h"
#include "izanami-worker.h"
#include "operation.h"
#include "workerexecutor.h"

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

void configworker(struct networkserver *server) {

	dictionary *dict = getdict();
	server->port = iniparser_getint(dict, IZANAMI_WORKER_PORT, 8000);
	server->eth = iniparser_getstring(dict, IZANAMI_WORKER_ETH, "0.0.0.0");
	server->maxconn = iniparser_getint(dict, IZANAMI_WORKER_MAXCONN, 1024);
	server->executor = (struct executor*) initworkerexecutor();
}

static enum operation reportop = report;
void *initreportthread(void *args) {

	struct worker *_worker = (struct worker *) args;
	dictionary *dict = getdict();
	int period = iniparser_getint(dict, IZANAMI_WORKER_REPORT_PERIOD, 10 * 60);

	int masterport = iniparser_getint(dict, IZANAMI_MASTER_PORT, 7000);
	char *mastereth = iniparser_getstring(dict, IZANAMI_MASTER_ETH, "0.0.0.0");
	int masterfd = socket(AF_INET, SOCK_STREAM, 0);
	struct sockaddr_in addr;
	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(masterport);
	addr.sin_addr.s_addr = inet_addr(mastereth);
	connect(masterfd, &addr, sizeof(addr));

	enum clienttype type = worker;
	send(masterfd, &type, sizeof(type), 0);
	while (TRUE) {

		setiregioninfoset(_worker->set, _worker->datadir);

		send(masterfd, &reportop, sizeof(report), 0);
		send(masterfd, &(_worker->set->num), sizeof(int), 0);
		send(masterfd, _worker->set->infos,
				_worker->set->num * sizeof(struct iregioninfo), 0);

		sleep(period);
	}
	return NULL;
}

void configworkerset(struct iregioninfoset* set) {

	dictionary *dict = getdict();
	set->maxnum = iniparser_getint(dict, IZANAMI_WORKER_REGMAXNUM, 1024);
	if (set->maxnum > IZANAMI_WOEKER_REGMAXNUM_MAX) {
		set->maxnum = IZANAMI_WOEKER_REGMAXNUM_MAX;
	}

	set->num = 0;
}

struct worker *initworker() {

	struct worker *_worker = (struct worker *) malloc(sizeof(struct worker));
	_worker->networkserver = initnetworkserver(configworker);
	_worker->set = initiregioninfoset(configworkerset);
	_worker->datadir = iniparser_getstring(getdict(), IZANAMI_WORKER_DATADIR,
			"/izanami/data");

	_worker->networkserver->executor = (struct executor *) initworkerexecutor();
	((struct workerexecutor *) _worker->networkserver->executor)->server =
			_worker;

	// 初始化与izanami-master通信的线程
	pthread_create(&(_worker->reportthread), NULL, initreportthread, _worker);

	return _worker;
}

void waitworkerfinish(struct worker *_worker) {

	printf("waiting worker to finish\n");
	pthread_join(_worker->networkserver->serverthread, NULL);
}
