/*
 * readaction.h
 *
 *  Created on: 2018年9月10日
 *      Author: caojx
 */

#ifndef READACTION_H_
#define READACTION_H_

#include "config.h"
#include "icell.h"
#include "ifile.h"
#include "skiplist.h"

enum readtype {

	compact, scanread
};

typedef void (*seek)(void *reader);
typedef struct icell* (*iterate)(void *reader);

struct reader {

	struct icell *current;
	void *nextreader;
	seek seekfun;
	iterate iteratefun;
};

struct filereader {

	struct icell *current;
	void *nextreader;
	seek seekfun;
	iterate iteratefun;

	int fd;
	int offset;
	char *buf;
	struct ifile *ifile;
};

struct memreader {

	struct icell *currnet;
	void *nextreader;
	seek seekfun;
	iterate iteratefun;

	struct leafinode *inode;
};

struct readaction {

	int clientfd;
	enum readtype type;
	int filereadercnt;
	struct iregion *iregion;

	struct filereader *readers;
	struct memreader *memreader;
	struct reader *readerlist;
};

struct readaction *initreadaction(struct iregion *iregion, int clientfd);
void doread(struct readaction *action);
void cleanaction(struct readaction *action);

#endif /* READACTION_H_ */
