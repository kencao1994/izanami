/*
 * main.c
 *
 *  Created on: 2018年8月11日
 *      Author: caojx
 */

#include "izanami-master.h"

int main() {

	struct master *master = initmaster();
	int i = 0;
	i = 1;
	waitmasterfinish(master);
	return 0;
}

