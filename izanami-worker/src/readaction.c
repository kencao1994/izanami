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

struct readaction *initreadaction(struct iregion *iregion, int clientfd) {

	struct ifilemanager *filemanager = getifilemanager();
	struct ifile *start = getifiles(filemanager, iregion->dirname);
	struct ifile *tmp = start;

	iregion->memstorerefcnt--;
	struct readaction *action = malloc(sizeof(struct readaction));
	action->clientfd = clientfd;
	action->type = scan;
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
			reader->nextreader = reader + 1;
			reader->buf = malloc(getfileblocksize());
			index++;
		}
		tmp = tmp->postfile;
	}

	return action;
}

void doread(struct readaction *action) {
}

void cleanaction(struct readaction *action) {

	close(action->clientfd);
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

