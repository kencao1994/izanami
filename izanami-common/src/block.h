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

#endif /* BLOCK_H_ */
