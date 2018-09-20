/*
 * ifile.h
 *
 *  Created on: 2018年9月10日
 *      Author: caojx
 */

#ifndef IFILE_H_
#define IFILE_H_

#include "config.h"
#include "izanami-worker.h"

enum ifilestatus {

	constructing, constructed, deletable
};

struct ifile {

	enum ifilestatus status;
	int readrefcnt;
	char filename[IZANAMI_FILE_MAXLEN];

	struct ifile *prefile;
	struct ifile *postfile;
};

struct ifilemanager {

	int filecnt;
	int maxfilecnt;
	struct ifile *files;
};

typedef void (*configifilemanager)(struct ifilemanager *manager);

struct ifilemanager *initifilemanager(struct worker *worker,
		configifilemanager config);
struct ifile *initifile(struct ifilemanager *manager, const char *filename,
		enum ifilestatus status);
void deleteifile(struct ifilemanager *manager, struct ifile *file);
struct iflie *getifiles(struct ifilemanager *manager, const char *dirname);
char *getrandfilename();

#endif /* IFILE_H_ */
