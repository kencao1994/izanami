/*
 * common.c
 *
 *  Created on: 2018年8月11日
 *      Author: caojx
 */

#include "common.h"
#include "config.h"
#include "iniparser.h"

static dictionary *dict = NULL;

dictionary *getdict() {

	if (dict == NULL) {
		dict = iniparser_load(IZANAMI_CONFIG);
	}

	return dict;
}
