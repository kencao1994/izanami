/*
 * iregioninfo.c
 *
 *  Created on: 2018年8月13日
 *      Author: caojx
 */

#include "config.h"
#include "iregioninfo.h"

#include <dirent.h>
#include <fcntl.h>
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
 *			  	/reg1
 *			  	/reg2
 *			  /tab2
 * 			/reg1
 * 			/reg2
 */

void cleariregioninfoset(struct iregioninfoset *set) {

	set->num = 0;
	memset(set->infos, 0, sizeof(struct iregioninfo) * set->maxnum);
}

static char filename[IZANAMI_MAX_LEN];
void setiregioninfoset(struct iregioninfoset *set, const char *filepath) {

	memset(filename, 0, IZANAMI_MAX_LEN);
	strcpy(filename, filepath);
	int filepathlen = strlen(filename);
	cleariregioninfoset(set);

	while (set->num < set->maxnum) {

		DIR *datadir = opendir(filepath);
		struct dirent *tabdirent = NULL;
		while ((tabdirent = readdir(datadir)) != NULL) {

			if (tabdirent->d_type == DT_DIR) {

				if (strcmp(tabdirent->d_name, IZANAMI_THIS_DIR) == 0
						|| strcmp(tabdirent->d_name, IZANAMI_PARENT_DIR) == 0) {
					continue;
				}

				char *tablenameptr = tabdirent->d_name;
				filename[filepathlen] = '/';
				strcat(filename + filepathlen + 1, tablenameptr);
				int tablenamelen = strlen(filename);

				DIR *tabdir = opendir(filename);
				struct dirent *regdirent = NULL;
				while ((regdirent = readdir(tabdir)) != NULL
						&& set->num < set->maxnum) {
					filename[tablenamelen] = '/';
					if (regdirent->d_type == DT_DIR) {
						if (strcmp(regdirent->d_name, IZANAMI_THIS_DIR) == 0
								|| strcmp(regdirent->d_name, IZANAMI_PARENT_DIR)
										== 0) {
							continue;
						}

						struct iregioninfo *info = set->infos + (set->num);
						set->num++;
						char *regionnameptr = regdirent->d_name;
						strcat(filename + tablenamelen + 1, regionnameptr);
						int regionnamelen = strlen(filename);
						filename[regionnamelen] = '/';
						strcat(filename + regionnamelen + 1,
						IZANAMI_REGION_BOUNDARY_FILE);
						int fd = open(filename, O_RDONLY);

						//初始化region相关信息
						strcpy(info->tablename, tablenameptr);
						strcpy(info->regionname, regionnameptr);
						readline(fd, info->startkey, IZANAMI_MAX_KEY_LEN);
						readline(fd, info->endkey, IZANAMI_MAX_KEY_LEN);

						memset(filename + tablenamelen, 0,
						IZANAMI_MAX_LEN - tablenamelen);
						close(fd);
					}
				}
				memset(filename + filepathlen, 0,
				IZANAMI_MAX_LEN - filepathlen);
				closedir(tabdir);
			}
		}
		closedir(datadir);
		break;
	}
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

