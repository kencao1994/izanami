/*
 * masteriregioninfo.c
 *
 *  Created on: 2018年8月16日
 *      Author: caojx
 */

#include "iregioninfo.h"
#include "masteriregioninfo.h"

void recviregionfromworker(struct master *_master, int num, struct iregioninfo *regbuf) {

	int tmp = 0;
	struct iregioninfoset *set = _master->set;
	while (tmp < num) {
		readfromreport(set->infos + set->num, regbuf + tmp);
		set->num ++;
		tmp ++;
	}

}
