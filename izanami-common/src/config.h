/*
 * config.h
 *
 *  Created on: 2018年8月11日
 *      Author: caojx
 */

#ifndef SRC_CONFIG_H_
#define SRC_CONFIG_H_

#define IZANAMI_CONFIG "/izanami/config"
#define IZANAMI_THIS_DIR "."
#define IZANAMI_PARENT_DIR ".."

#define IZANAMI_MAX_IP_LEN 1024

#define IZANAMI_MAX_LEN 4096
#define IZANAMI_MAX_KEY_LEN 512
#define IZANAMI_MAX_TAB_LEN 128

#define IZANAMI_MASTER_ETH "master:eth"
#define IZANAMI_MASTER_PORT "master:port"
#define IZANAMI_MASTER_MAXCONN "master:maxconn"
#define IZANAMI_MASTER_MAXWORKER "master:maxworker"
#define IZANAMI_MASTER_TABLE_MAXNUM "master:maxtablenum"
#define IZANAMI_MASTER_TABLE_NAMEFILE "master:tablenames"

#define IZANAMI_MEMPOOL_MAXSIZE "mempool:maxsize"
#define IZANAMI_MEMPOOL_BLOCKSIZE "mempool:blocksize"
#define IZANAMI_MEMPOOL_METASIZE "mempool:metasize"
#define IZANAMI_MEMPOOL_BLOCKDATASIZE "mempool:datasize"

#define IZANAMI_REGION_NAME_LEN 32
#define IZANAMI_REGION_BOUNDARY_FILE "boundary"
#define IZANAMI_READY_SLEEP_TIME 1

#define IZANAMI_SKIPLIST_MAXSKIP "skiplist:maxskip"
#define IZANAMI_SKIPLIST_MAXLAYER "skiplist:maxlayers"

#define IZANAMI_WORKER_ETH "worker:eth"
#define IZANAMI_WORKER_PORT "worker:port"
#define IZANAMI_WORKER_MAXCONN "worker:maxconn"
#define IZANAMI_WORKER_REPORT_PERIOD "worker:reportperiod"
#define IZANAMI_WORKER_REGMAXNUM "worker:maxregnum"
#define IZANAMI_WOEKER_REGMAXNUM_MAX 1024
#define IZANAMI_WORKER_DATADIR "worker:datadir"

#define IZANAMI_FILE_MAXLEN 1024
#define IZANAMI_FILE_MAXNUM "file:maxnum"
#define IZANAMI_FILE_BLOCKSIZE "file:blocksize"
#define IZANAMI_FILE_BLOCKMETASIZE "file:blockmetasize"

#endif /* SRC_CONFIG_H_ */
