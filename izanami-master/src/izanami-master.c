/*
 * izanami-master.c
 *
 *  Created on: 2018年8月11日
 *      Author: caojx
 */

#include "clienttype.h"
#include "common.h"
#include "config.h"
#include "dictionary.h"
#include "executor.h"
#include "iniparser.h"
#include "iregioninfo.h"
#include "izanami-master.h"
#include "masterexecutor.h"
#include "networkserver.h"

#include <arpa/inet.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

void waitmasterfinish(struct master *_master) {

	printf("waiting master to finish\n");
	pthread_join(_master->networkserver->serverthread, NULL);
}

void onmasterconnect(struct networkserver *server, struct epoll_event *event,
		struct sockaddr_in *addr) {

	struct masterexecutor *executor = (struct masterexecutor *) server->executor;
	int clientfd = event->data.fd;
	char *ip = inet_ntoa(addr->sin_addr);
	enum clienttype type;
	recv(clientfd, &type, sizeof(enum clienttype), 0);

	if (type == worker) {
		struct mastersideworker *workeragent = getmastersideworkerbyfd(
				executor->server->workermanager, clientfd);

		if (workeragent == NULL) {
			addmastersideworker(executor->server->workermanager, clientfd, ip);
		}
	}

}

void configmaster(struct networkserver *server) {

	dictionary *dict = getdict();

	server->port = iniparser_getint(dict, IZANAMI_MASTER_PORT, 7000);
	server->eth = iniparser_getstring(dict, IZANAMI_MASTER_ETH, "0.0.0.0");
	server->maxconn = iniparser_getint(dict, IZANAMI_MASTER_MAXCONN, 1024);
	server->executor = (struct executor*) initmasterexecutor();
	server->connfun = onmasterconnect;
}

void configmasterset(struct iregioninfoset *set) {

	dictionary *dict = getdict();
	set->maxnum = iniparser_getint(dict, IZANAMI_WORKER_REGMAXNUM, 1024);
	set->num = 0;
}

void configmastersideworkermanager(struct mastersideworkermanager *manager) {

	dictionary *dict = getdict();
	manager->maxworkernum = iniparser_getint(dict, IZANAMI_MASTER_MAXWORKER,
			512);
	manager->workernum = 0;
	manager->workers = (struct mastersideworker *) malloc(
			sizeof(struct mastersideworker) * manager->maxworkernum);
	int index = 0;
	for (; index < manager->maxworkernum; index++) {
		struct mastersideworker *worker = manager->workers + index;
		worker->set = initiregioninfoset(configmasterset);
	}
}

struct master *initmaster() {

	struct master *_master = (struct master *) malloc(sizeof(struct master));
	_master->networkserver = initnetworkserver(configmaster);
	_master->workermanager = initmastersideworkermanager(
			configmastersideworkermanager);
	struct masterexecutor * executor =
			(struct masterexecutor *) _master->networkserver->executor;
	executor->server = _master;

	_master->tabmanager = inittablemanager();

	setready();
	return _master;
}

void docreatetable(struct master *_master, char *tablename, char **splits) {

	inserttable(_master->tabmanager, tablename);
}
