/*
 * main.c
 *
 *  Created on: 2018年8月12日
 *      Author: caojx
 */

#include "izanami-worker.h"

int main() {

	struct worker *_worker = initworker();
	waitworkerfinish(_worker);
	return 0;
}
