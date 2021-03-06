/*
 * tablemanager.c
 *
 *  Created on: 2018年8月26日
 *      Author: caojx
 */

#include "common.h"
#include "config.h"
#include "tablemanager.h"

#include <fcntl.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>

void settablename(struct tablename *tablename, const char *name) {

	strcpy(tablename->name, name);
}

int tablenamecmp(void *arg1, void *arg2) {

	struct tablename *tab1 = (struct tablename *) arg1;
	struct tablename *tab2 = (struct tablename *) arg2;

	return strcmp(tab1->name, tab2->name);
}

struct tablemanager *inittablemanager() {

	dictionary *dict = getdict();
	struct tablemanager *manager = (struct tablemanager *) malloc(
			sizeof(struct tablemanager));
	manager->maxtablenum = iniparser_getint(dict, IZANAMI_MASTER_TABLE_MAXNUM,
			1024);
	manager->tablenum = 0;
	manager->tables = (struct tablename *) malloc(
			sizeof(struct tablename) * manager->maxtablenum);

	loadtablename(manager);
	return manager;
}

int exists(struct tablemanager *manager, struct tablename *table) {

	return halffind(manager->tables, sizeof(struct tablename), 0,
			manager->tablenum, table, tablenamecmp);
}

static char namebuf[IZANAMI_MAX_TAB_LEN];
void loadtablename(struct tablemanager *manager) {

	dictionary *dict = getdict();
	char *tablenamefile = iniparser_getstring(dict,
			IZANAMI_MASTER_TABLE_NAMEFILE, "tablenames");
	int fd = open(tablenamefile, O_RDWR);

	int index = 0;
	do {
		readline(fd, namebuf, IZANAMI_MAX_TAB_LEN);
		if (strlen(namebuf) == 0 || manager->tablenum >= manager->maxtablenum) {
			break;
		}

		struct tablename *name = manager->tables + index;
		manager->tablenum++;
		settablename(name, namebuf);
		index++;
	} while (TRUE);

	close(fd);
	qsort(manager->tables, manager->tablenum, sizeof(struct tablename),
			tablenamecmp);
}

void savetablename(struct tablemanager *manager) {

	dictionary *dict = getdict();
	char *tablenamefile = iniparser_getstring(dict,
			IZANAMI_MASTER_TABLE_NAMEFILE, "tablenames");
	int fd = open(tablenamefile, O_RDWR);

	int i = 0;
	for (; i < manager->tablenum; i++) {
		struct tablename *tab = manager->tables + i;
		write(fd, tab->name, strlen(tab->name));
		if (i != manager->tablenum) {
			write(fd, '\n', sizeof(char));
		}
	}

	close(fd);
}

void inserttable(struct tablemanager *manager, char *table) {

	if (exists(manager, table) == -1) {
		struct tablename *name = manager->tables + manager->tablenum;
		manager->tablenum++;
		strcpy(name->name, table);

		qsort(manager->tables, manager->tablenum, sizeof(struct tablename),
				tablenamecmp);
	} else {

		printf("talbe with name [%s] exists \n", table);
	}

}
