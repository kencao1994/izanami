/*
 * icell.h
 *
 *  Created on: 2018年9月4日
 *      Author: caojx
 */

#ifndef ICELL_H_
#define ICELL_H_

#include "memconsumer.h"

/**
 *
 * icell 大致如下
 * column\value\key\mvcc
 */

struct icell {

	short keylen;
	short collen;
	short vallen;

};

struct icell *fromptr(void *ptr);
int icellsize(struct icell *icell);
int icellcmp(void *left, void *right);

long getmvcc(struct icell *icell);
void getkey(struct icell *icell, char *buf);
void getcol(struct icell *icell, char *buf);
void getval(struct icell *icell, char *buf);

long generatemvcc();
struct icell *randicellfromcousumer(struct memconsumer *consumer);
void setmvcc(struct icell *icell, long mvcc);
#endif /* ICELL_H_ */
