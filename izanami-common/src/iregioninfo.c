/*
 * iregioninfo.c
 *
 *  Created on: 2018年8月13日
 *      Author: caojx
 */

#include "iregioninfo.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int regioncmp(void *arg1, void *arg2) {

	struct iregioninfo *reg1 = (struct iregioninfo *) arg1;
	struct iregioninfo *reg2 = (struct iregioninfo *) arg2;

	int ret = 0;
	ret = strcmp(reg1->tablename, reg2->tablename);

	if (ret == 0) {
		ret = strcmp(reg1->startkey, reg2->startkey);
	}

	if (ret == 0) {
		printf(
				"error compare region1(%s, %s, %s, %s) and region2(%s, %s, %s, %s)\n",
				reg1->tablename, reg1->regionname, reg1->startkey, reg1->endkey,
				reg2->tablename, reg2->regionname, reg2->startkey,
				reg2->endkey);
	}

	return ret;
}

//从文件中获取iregion 相关信息
/**
 *
 *  理论上数据目录如下
 *	/datadir/
 *			  /tab1
 *			  		/reg1
 *			  		/reg2
 *			  /tab2
 * 				/reg1
 * 				/reg2
 */

void cleariregioninfoset(struct iregioninfoset *set) {

	set->num = 0;
	memset(set->infos, 0, sizeof(struct iregioninfo) * set->maxnum);
}

void setiregioninfoset(struct iregioninfoset *set, const char *filepath) {

	cleariregioninfoset(set);

}

//从worker上报的消息中获取iregioninfo
void readfromreport(struct iregioninfo *info, void *arg) {

	struct iregioninfo *tmp = (struct iregioninfo *) arg;
	strcpy(info->tablename, tmp->tablename);
	strcpy(info->regionname, tmp->regionname);
	strcpy(info->startkey, tmp->startkey);
	strcpy(info->endkey, tmp->endkey);
}

struct iregioninfoset *initiregioninfoset(configiregioninfoset fn) {

	struct iregioninfoset *set = (struct iregioninfoset *) malloc(
			sizeof(struct iregioninfoset));
	fn(set);
	set->infos = (struct iregioninfo *) malloc(
			sizeof(struct iregioninfo) * set->maxnum);
	return set;
}

