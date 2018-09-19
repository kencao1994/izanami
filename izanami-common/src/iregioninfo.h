/*
 * iregioninfo.h
 *
 *  Created on: 2018年8月13日
 *      Author: caojx
 */

#ifndef IREGIONINFO_H_
#define IREGIONINFO_H_

#include "config.h"

struct iregioninfo {

	char startkey[IZANAMI_MAX_KEY_LEN];
	char endkey[IZANAMI_MAX_KEY_LEN];
	char tablename[IZANAMI_MAX_TAB_LEN];
	char regionname[IZANAMI_REGION_NAME_LEN];
};

int regioncmp(void *arg1, void *arg2);
//从worker上报的消息中获取iregioninfo
void readfromreport(struct iregioninfo* info, void *arg);
char *getregiondir(struct iregioninfo *info);
char *getregiondatadir(const char *regiondir);

struct iregioninfoset {

	int num;
	int maxnum;
	struct iregioninfo *infos;
};

//设置最大region数，初始化infos堆内存
typedef void (*configiregioninfoset)(struct iregioninfoset* set);
struct iregioninfoset *initiregioninfoset(configiregioninfoset fn);

//从文件中获取iregion 相关信息
void setiregioninfoset(struct iregioninfoset* set, const char *filepath);

#endif /* IREGIONINFO_H_ */
