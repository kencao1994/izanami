/*
 * masteriregioninfo.c
 *
 *  Created on: 2018年8月16日
 *      Author: caojx
 */

#include "iregioninfo.h"
#include "masteriregioninfo.h"

void recviregionfromworker(struct iregioninfoset *set, int num, struct iregioninfo *regbuf) {

	int tmp = 0;
	while (tmp < num) {
		readfromreport(set->infos + set->num, regbuf + tmp);
		set->num ++;
		tmp ++;
	}

}
