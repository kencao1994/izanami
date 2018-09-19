/*
 * ifile.c
 *
 *  Created on: 2018年9月10日
 *      Author: caojx
 */

#include "common.h"
#include "ifile.h"

#include <dirent.h>
#include <stdlib.h>

static char ifilename[IZANAMI_MAX_LEN] = {'\0'};
struct ifilemanager *initifilemanager(struct worker *worker,
		configifilemanager config) {

	struct ifilemanager *manager = (struct ifilemanager *) malloc(
			sizeof(struct ifilemanager));
	config(manager);

	setiregioninfoset(worker->set, worker->datadir);

	int i = 0;
	for (; i < worker->set->num; i++) {
		struct iregioninfo *info = worker->set->infos + i;
		char *name = getregiondir(info);
		char *data = getregiondatadir(name);
		DIR *datadir = opendir(data);

		struct dirent *datafile = NULL;
		while ((datafile = readdir(datadir)) != NULL) {

			if (datafile->d_type == DT_REG) {
				memset(ifilename, 0, IZANAMI_MAX_LEN);
				strcpy(ifilename, data);
				ifilename[strlen(ifilename)] = '/';
				strcpy(ifilename + strlen(ifilename), datafile->d_name);

				initifile(manager, ifilename);
			}
		}

	}

	return manager;
}

int filenamecmp(const char *file1, const char *file2) {

	if (file1 == NULL)
		return -1;

	return strcmp(file1, file2);
}

struct ifile *initifile(struct ifilemanager *manager, const char *filename) {

	if (manager->filecnt >= manager->maxfilecnt) {
		return NULL;
	}

	struct ifile *file = (struct ifile *) malloc(sizeof(struct ifile));
	file->status = constructing;
	strcpy(file->filename, filename);
	file->readrefcnt = 0;
	file->prefile = file->postfile = NULL;

	struct ifile *ptr = manager->files;
	struct ifile *preptr = manager->files;
	int index = 0;
	while (index < manager->filecnt && filenamecmp(ptr->filename, filename) < 0) {

		index++;
		preptr = ptr;
		ptr = ptr->postfile;
	}

	if (index == 0) {
		manager->files = file;
	} else {
		preptr->postfile = file;
		ptr->prefile = file;
		file->prefile = preptr;
		file->postfile = ptr;
	}

	manager->filecnt++;

	return file;
}

void deleteifile(struct ifilemanager *manager, struct ifile *file) {

	struct ifile *ptr = manager->files;
	int ret = 0;
	while (ptr != NULL && (ret = filenamecmp(ptr->filename, file->filename)) < 0) {

		ptr = ptr->postfile;
	}
	if (ret == 0) {
		ptr->status = deletable;
	}
}

int isprefix(const char *filename, const char *dirname) {

	int ret = 0;

	int flen = strlen(filename);
	int dlen = strlen(dirname);

	char *f = filename;
	char *d = dirname;

	while (*f != '\0' && *d != '\0' && *f == *d) {

		f++;
		d++;
		ret++;
	}

	return ret == dlen;
}

struct iflie *getifiles(struct ifilemanager *manager, const char *dirname) {

	struct ifile *start = manager->files;
	int cnt = 0;
	int ret = 0;
	while (cnt < manager->filecnt && (ret = isprefix(start, dirname)) != 0) {

		cnt++;
		start = start->postfile;
	}

	if (ret != 0)
		return NULL;
	else
		return start;
}
