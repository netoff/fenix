#pragma once

#include <stdio.h>

#include <httpd.h>
#include "apr_tables.h"
#include "apr_hash.h"

struct fenix_server_conf
{
	apr_pool_t *pool;
	
	#if APR_HAS_THREADS
	apr_thread_mutex_t *mutex;
	#endif
	
	apr_hash_t *cache;
};

void* fenix_create_server_conf(apr_pool_t* apc_pool, char* x);
