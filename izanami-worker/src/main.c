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

int main() {

	struct worker *_worker = initworker();
	waitworkerfinish(_worker);
	return 0;
}
