/*
 * main.c
 *
 *  Created on: 2018年8月12日
 *      Author: caojx
 */

#include "iregioninfo.h"
#include "izanami-worker.h"

#include <stdlib.h>
#include <time.h>

void configset(struct iregioninfoset* set) {

	set->maxnum = 10;
	set->num = 0;
}

int main() {

	struct iregioninfoset *set = initiregioninfoset(configset);
	setiregioninfoset(set, "/izanami/data");


	srand(time(NULL));

	struct worker *_worker = initworker();
	waitworkerfinish(_worker);
	return 0;
}
