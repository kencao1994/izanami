/*
 * main.c
 *
 *  Created on: 2018年8月12日
 *      Author: caojx
 */

#include "izanami-worker.h"

#include <stdlib.h>
#include <time.h>

int main() {

	srand(time(NULL));

	struct worker *_worker = initworker();
	waitworkerfinish(_worker);
	return 0;
}
