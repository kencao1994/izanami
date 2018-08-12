/*
 * executor.h
 *
 *  Created on: 2018年8月11日
 *      Author: caojx
 */

#ifndef SRC_EXECUTOR_H_
#define SRC_EXECUTOR_H_

#include "common.h"

#define EXECUTOR void

struct executor {

	void (*execute)(EXECUTOR *this, int fd);
};

#endif /* SRC_EXECUTOR_H_ */
