/*
 * block.h
 *
 *  Created on: 2018年8月28日
 *      Author: caojx
 */

#ifndef BLOCK_H_
#define BLOCK_H_

struct consumerblock {

	int dataused;
	struct consumerblock *next;
};

struct fileblockbuf {

	int dataused;
	int icellcount;

};

struct fileblockbuf *initfileblockbuf(void *buf);
void destroyfileblockbuf(struct fileblockbuf *buf);

#endif /* BLOCK_H_ */
