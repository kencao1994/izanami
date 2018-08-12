/*
 * ianami-worker.c
 *
 *  Created on: 2018年8月12日
 *      Author: caojx
 */

#include "common.h"
#include "config.h"
#include "dictionary.h"
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

struct master * initmaster() {

	return NULL;
}

void waitworkerfinish(struct worker *_worker) {

	printf("waiting worker to finish\n");
	pthread_join(_worker->networkserver->serverthread, NULL);
}
