#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include "clienttype.h"
#include "icell.h"
#include "operation.h"

#define MAXLINE 4096

int main(int argc, char** argv) {
	int sockfd, workerfd;
	char buf[MAXLINE];
	struct sockaddr_in servaddr, workeraddr;

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	workerfd = socket(AF_INET, SOCK_STREAM, 0);

	memset(&servaddr, 0, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(7001);
	inet_pton(AF_INET, "127.0.0.1", &servaddr.sin_addr);

	memset(&workeraddr, 0, sizeof(workeraddr));
	workeraddr.sin_family = AF_INET;
	workeraddr.sin_port = htons(8009);
	inet_pton(AF_INET, "127.0.0.1", &workeraddr.sin_addr);

	connect(sockfd, (struct sockaddr*) &servaddr, sizeof(servaddr));
	connect(workerfd, (struct sockaddr*) &workeraddr, sizeof(workeraddr));

	enum clienttype type = client;
	send(sockfd, &type, sizeof(enum clienttype), 0);

	enum operation op = route;
	send(sockfd, &op, sizeof(enum operation), 0);

	enum operation oscan = scan ;
	send(workerfd, &oscan, sizeof(oscan), 0);

	send(workerfd, "tab1", 4, 0);
	send(workerfd, "0", 1, 0);
	send(workerfd, "9", 1, 0);

//	while (1) {
//		enum operation oput = put;
//		send(workerfd, &oput, sizeof(oput), 0);
//		char *table = "tab1";
//		send(workerfd, table, 4, 0);
//
//		int keylen = 1;
//		int collen = 10;
//		int vallen = 100;
//		int len = keylen + collen + vallen + 3 * sizeof(short) + sizeof(long);
//		struct icell *cell = malloc(len);
//		cell->keylen = keylen;
//		cell->collen = collen;
//		cell->vallen = vallen;
//		randstr((void *) cell + icellheadlen(cell), cell->collen);
//		randstr((void *) cell + icellheadlen(cell) + cell->collen,
//				cell->vallen);
//		randstr(
//				(void *) cell + icellheadlen(cell) + cell->collen
//						+ cell->vallen, cell->keylen);
//		send(workerfd, cell, len, 0);
//
//		free(cell);
//	}
	close(sockfd);
	close(workerfd);
	exit(0);
}
