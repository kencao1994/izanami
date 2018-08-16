/*
 * masteriregion.h
 *
 *  Created on: 2018年8月16日
 *      Author: caojx
 */

#ifndef MASTERIREGION_H_
#define MASTERIREGION_H_

#include "iregioninfo.h"
#include "izanami-master.h"

void recviregionfromworker(struct master *_master, int num, struct iregioninfo *regbuf);

#endif /* MASTERIREGION_H_ */
