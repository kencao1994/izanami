/*
 * ifile.c
 *
 *  Created on: 2018年9月10日
 *      Author: caojx
 */

#include "common.h"
#include "ifile.h"

#include <stdlib.h>

struct ifilemanager *initifilemanager(struct worker *worker, configifilemanager config) {

	struct ifilemanager *manager = (struct ifilemanager *) malloc(
			sizeof(struct ifilemanager));
	config(manager);
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

	manager->filecnt++;
	struct ifile *ptr = manager->files;
	struct ifile *preptr = manager->files;
	while (ptr != NULL && filenamecmp(ptr->filename, filename) < 0) {

		preptr = ptr;
		ptr = ptr->postfile;
	}

	if (preptr == NULL) {
		manager->files = file;
	} else {
		preptr->postfile = file;
		ptr->prefile = file;
		file->prefile = preptr;
		file->postfile = ptr;
	}

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
