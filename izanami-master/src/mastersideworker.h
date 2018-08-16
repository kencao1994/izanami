/*
 * mastersideworker.h
 *
 *  Created on: 2018年8月16日
 *      Author: caojx
 */

#ifndef MASTERSIDEWORKER_H_
#define MASTERSIDEWORKER_H_

#include "iregioninfo.h"

struct mastersideworker {

	char *ip;
	int port;
	int workerfd;
	struct iregioninfoset *set;
};

int mastersideworkercmp(void *arg1, void *arg2);
struct mastersideworker *initmastersideworker(configiregioninfoset config);

#endif /* MASTERSIDEWORKER_H_ */
