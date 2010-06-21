#include "server_conf.h"

void* fenix_create_server_conf(apr_pool_t* pool, char* x)
{
	apr_status_t ret;
	
	fenix_server_conf* server_conf = (fenix_server_conf*)apr_pcalloc(pool, sizeof(fenix_server_conf));
	
	server_conf->pool = pool;
	
	#if APR_HAS_THREADS
		apr_thread_mutex_create(&server_conf->mutex, APR_THREAD_MUTEX_DEFAULT, server_conf->pool);
		
		/*if (ret != APR_SUCCESS) 
		{
			ap_log_perror(APLOG_MARK, APLOG_DBG, ret, pool, "Failed to create mutex for fenix_module");
			return;
		}*/
	#endif
	
	server_conf->cache = apr_hash_make(server_conf->pool);
	
	return server_conf;
}
