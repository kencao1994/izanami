/*
 * readaction.h
 *
 *  Created on: 2018年9月10日
 *      Author: caojx
 */

#ifndef READACTION_H_
#define READACTION_H_

#include "config.h"
#include "ifile.h"

enum readtype {

	compact,
	scan
};

struct filereader {

	int fd;
	char *buf;
	struct filereader *nextreader;
	struct ifile *ifile;
};

struct readaction {

	enum readtype type;
	int filereadercnt;
	struct filereader *readers;
};

struct readaction *initreadaction(const char *dirname);
void doread(struct readaction *action);
void cleanaction(struct readaction *action);

#endif /* READACTION_H_ */
