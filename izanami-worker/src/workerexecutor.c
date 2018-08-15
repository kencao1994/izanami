/*
 * workerexecutor.c
 *
 *  Created on: 2018年8月12日
 *      Author: caojx
 */

#include "workerexecutor.h"

#include <stdlib.h>

struct workerexecutor *initworkerexecutor() {

	struct workerexecutor *executor = (struct workerexecutor *)malloc(sizeof(struct workerexecutor));

	return executor;
}
