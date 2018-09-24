/*
 * readaction.c
 *
 *  Created on: 2018年9月10日
 *      Author: caojx
 */

#include "common.h"
#include "iregion.h"
#include "izanami-worker.h"
#include "readaction.h"

#include <fcntl.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>

void filestoreseek(struct filereader *reader) {

	reader->current = ((void *) reader->buf + reader->offset);
}

void filestoreiter(struct filereader *reader) {

	struct fileblockbuf *buf = (struct fileblockbuf *) reader->buf;
	int ret = -1;
	if (reader->offset - getfileblockmetasize() + icellsize(reader->current)
			== buf->dataused) {
		reader->offset = getfileblockmetasize();
		ret = read(reader->fd, reader->buf, getfileblocksize());
	}

	if (ret == 0) {
		reader->current = NULL;
	} else {
		reader->offset += icellsize(reader->current);
		filestoreseek(reader);
	}
}

void memstoreseek(struct memreader *reader) {

	if (reader->inode == NULL)
		reader->currnet = NULL;
	else
		reader->currnet = reader->inode->element;
}

void memstoreiter(struct memreader *reader) {

	reader->inode = reader->inode->post;
	if (reader->inode == NULL) {
		reader->currnet = NULL;
	} else {
		memstoreseek(reader);
	}
}

struct readaction *initreadaction(struct iregion *iregion, int clientfd) {

	struct ifilemanager *filemanager = getifilemanager();
	struct ifile *start = getifiles(filemanager, iregion->dirname);
	struct ifile *tmp = start;

	struct readaction *action = malloc(sizeof(struct readaction));
	iregion->memstorerefcnt++;
	action->memreader = malloc(sizeof(struct memreader));
	action->memreader->iteratefun = memstoreiter;
	action->memreader->seekfun = memstoreseek;
	struct skiplist *list = iregion->memstore;
	struct steaminode *inode = list->routenode;
	while (inode->down != NULL) {
		inode = inode->down;
	}
	action->memreader->inode = inode->post;

	action->clientfd = clientfd;
	action->type = scanread;
	action->iregion = iregion;
	int cnt = 0;

	while (isprefix(start->filename, iregion->dirname)) {
		if (start->status == constructed) {
			cnt++;
			start->readrefcnt++;
		}
		start = start->postfile;
	}
	action->filereadercnt = cnt;
	action->readers = malloc(cnt * sizeof(struct filereader));

	int index = 0;
	while (isprefix(tmp->filename, iregion->dirname)) {
		if (tmp->status == constructed) {
			struct filereader *reader = action->readers + index;
			reader->ifile = tmp;
			reader->fd = open(tmp->filename, O_RDONLY);
			reader->buf = malloc(getfileblocksize());
			reader->offset = getfileblockmetasize();
			reader->iteratefun = filestoreiter;
			reader->seekfun = filestoreseek;

			memset(reader->buf, 0, getfileblocksize());
			read(reader->fd, reader->buf, getfileblocksize());
			index++;
		}
		tmp = tmp->postfile;
	}

	return action;
}

int readercmp(struct reader *tobeinserted, struct reader *examedone) {

	if (examedone->current == NULL) {
		return -1;
	} else if (tobeinserted->current == NULL) {
		return 1;
	} else {
		return icellcmp(tobeinserted->current, examedone->current);
	}
}

void insertreaderintoreaderlist(struct readaction *action, int cnt,
		struct reader *tobeinserted) {

	struct reader *examedone = action->readerlist;
	struct reader *onebefore = NULL;
	int index = 0;
	while (index < cnt && readercmp(tobeinserted, examedone) > 0) {
		onebefore = examedone;
		examedone = examedone->nextreader;
		index++;
	}

	if (onebefore == NULL) {
		tobeinserted->nextreader = action->readerlist;
		action->readerlist = tobeinserted;
	} else {
		tobeinserted->nextreader = onebefore->nextreader;
		onebefore->nextreader = tobeinserted;
	}

}

void doread(struct readaction *action) {

	action->memreader->seekfun(action->memreader);
	int index = 0;
	for (; index < action->filereadercnt; index++) {
		struct filereader *reader = action->readers + index;
		reader->seekfun(reader);
	}

	//对filereader进行排序
	int cnt = 1;
	action->readerlist = (struct reader *) action->readers;

	while (cnt < action->filereadercnt) {

		struct reader *tobeinserted = (struct reader *) (action->readers + cnt);
		insertreaderintoreaderlist(action, cnt, tobeinserted);
		cnt++;
	}

	//插入memstorereader
	insertreaderintoreaderlist(action, cnt,
			(struct reader *) action->memreader);

	struct icell *min = NULL;
	while ((min = action->readerlist->current) != NULL) {

		printicell(min);
		action->readerlist->iteratefun(action->readerlist);
		struct reader *pre = action->readerlist;
		action->readerlist = action->readerlist->nextreader;
		insertreaderintoreaderlist(action, action->filereadercnt, pre);
	}
}

void cleanaction(struct readaction *action) {

//	close(action->clientfd);
	int index = 0;
	while (index < action->filereadercnt) {

		struct filereader reader = action->readers[index];
		close(reader.fd);
		free(reader.buf);
		reader.ifile->readrefcnt--;

		index++;
	}

	action->iregion->memstorerefcnt--;
	free(action);
}

