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

#define IZANAMI_MAX_LEN 4096
#define IZANAMI_MAX_KEY_LEN 512
#define IZANAMI_MAX_TAB_LEN 128

#define IZANAMI_MASTER_ETH "master:eth"
#define IZANAMI_MASTER_PORT "master:port"
#define IZANAMI_MASTER_MAXCONN "master:maxconn"
#define IZANAMI_MASTER_MAXWORKER "master:maxworker"

#define IZANAMI_REGION_NAME_LEN 32
#define IZANAMI_REGION_BOUNDARY_FILE "boundary"

#define IZANAMI_WORKER_ETH "worker:eth"
#define IZANAMI_WORKER_PORT "worker:port"
#define IZANAMI_WORKER_MAXCONN "worker:maxconn"
#define IZANAMI_WORKER_REPORT_PERIOD "worker:reportperiod"
#define IZANAMI_WORKER_REGMAXNUM "worker:maxregnum"
#define IZANAMI_WOEKER_REGMAXNUM_MAX 1024
#define IZANAMI_WORKER_DATADIR "worker:datadir"

#endif /* SRC_CONFIG_H_ */
