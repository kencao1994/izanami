/*
 * icell.h
 *
 *  Created on: 2018年9月4日
 *      Author: caojx
 */

#ifndef ICELL_H_
#define ICELL_H_

struct icell {

	short keylen;
	short colen;
	short vallen;

};

struct icell *fromptr(void *ptr);
int icellsize(struct icell *icell);
int icellcmp(struct icell *arg1, struct icell *arg2);

#endif /* ICELL_H_ */
