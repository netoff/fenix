#include "mod_fenix.h"

#include <apr_dso.h>
#include <apr_file_info.h>
#include <http_log.h>

#include "debug.h"
#include "dir_conf.h"
#include "server_conf.h"
#include "directives.h"
#include "driver.h"
#include "handler.h"

/*
static void fenix_init(apr_pool_t* pool, server_rec* apc_server)
{
	apr_status_t ret;
	
	fenix_server_conf* server_conf = ap_get_module_config(apc_server->module_config, &fenix_module);
	server_conf->self = apc_server;
	
	server_conf->log_file = fopen("/opt/log/log.txt", "a");
	apr_pool_cleanup_register(server_conf->pool, server_conf->log_file, (void*)fclose, apr_pool_cleanup_null);
	
	if(server_conf->log_file)
	{
		fprintf(server_conf->log_file, "Inital write to log");
	}
	
	/*
	ret = apr_pool_create(&server_conf->pool, pool);
	
	if (ret != APR_SUCCESS) 
	{
		ap_log_perror(APLOG_MARK, APLOG_CRIT, ret, pool, "Failed to create subpool for fenix_module");
		return;
	}
	
	
	#if APR_HAS_THREADS
		ret = apr_thread_mutex_create(&server_conf->mutex, APR_THREAD_MUTEX_DEFAULT, server_conf->pool);
		
		if (ret != APR_SUCCESS) 
		{
			ap_log_perror(APLOG_MARK, APLOG_CRIT, ret, pool, "Failed to create mutex for fenix_module");
			return;
		}
	#endif
	
	server_conf->cache = apr_hash_make(server_conf->pool);
	
}*/
/*

static int fenix_post_config(apr_pool_t* pool, apr_pool_t* plog, apr_pool_t* ptemp, server_rec* apc_server)
{
	apr_status_t ret;
	
	fenix_server_conf* server_conf = ap_get_module_config(apc_server->module_config, &fenix_module);
	server_conf-> = apc_server;
	
	server_conf->log_file = fopen("/opt/log/log.txt", "a");
	apr_pool_cleanup_register(server_conf->pool, server_conf->log_file, (void*)fclose, apr_pool_cleanup_null);
	
	if(server_conf->log_file)
	{
		fprintf(server_conf->log_file, "Inital write to log");
	}
	
	return OK;
}*/

static apr_dso_handle_t* get_module(const char* dll_name, fenix_server_conf* server_conf)
{
	apr_status_t ret;
	apr_dso_handle_t* dll_module = NULL;
	apr_pool_t* pool = server_conf->pool;
	
	dll_module = (apr_dso_handle_t*)apr_hash_get(server_conf->cache, dll_name, APR_HASH_KEY_STRING);
	
	if(dll_module == NULL)
	{
		ret = apr_thread_mutex_lock(server_conf->mutex);
				
		if (ret != APR_SUCCESS) 
		{
			ap_log_perror(APLOG_MARK, APLOG_ERR, ret, pool, "Failed to acquire thread mutex");
			
			return HTTP_SERVICE_UNAVAILABLE;
		}
		
		dll_module = (apr_dso_handle_t*)apr_hash_get(server_conf->cache, dll_name, APR_HASH_KEY_STRING);			
		
		if (dll_module == NULL) 
		{
			ret = apr_dso_load(&dll_module, dll_name, server_conf->pool);
	
			if(ret != APR_SUCCESS)
			{
				char error[1024] = "";
		
				apr_dso_error(dll_module, error, sizeof(error));				
				ap_log_perror(APLOG_MARK, APLOG_ERR, ret, pool, "Failed loading application object: '%s'(%s)", dll_name, error);
				
				return HTTP_INTERNAL_SERVER_ERROR;
			}
			else
			{
				apr_hash_set(server_conf->cache, apr_pstrdup(pool, dll_name), APR_HASH_KEY_STRING, (const void*)dll_module);
				
				/*call init function every time you load module
				void (*init)(FILE*);
				ret = apr_dso_sym((void**)&init, dll_module, "appinit");
				if(ret != APR_SUCCESS)
				{
					ap_log_perror(APLOG_MARK, APLOG_ERR, 0, pool, "Failed calling module init function!");
				}
				init(server_conf->log_file);
				*/
				
				//Diagnostic message
				ap_log_perror(APLOG_MARK, APLOG_ERR, 0, pool, "Loaded application object file: '%s'", dll_name);
			}
		}
		
		ret = apr_thread_mutex_unlock(server_conf->mutex);
	
		if (ret != APR_SUCCESS) 
		{
			ap_log_perror(APLOG_MARK, APLOG_ERR, ret, pool, "Failed to release thread mutex!");
		}
	}
	
	return dll_module;
}

static int get_handle(shared_ptr<view::Response> (**handle)(const action::Request&),/*apr_pool_t* pool,*/ const char* dll_name, fenix_server_conf* server_conf)
{
	static const char* application_entry_point= "handle";
	
	apr_status_t ret;
	apr_dso_handle_t* dll_module;
	
	dll_module = get_module(dll_name, server_conf);
	
	if(dll_module != NULL)
	{
		ret = apr_dso_sym((void**)handle, dll_module, application_entry_point);
			
		if(ret != APR_SUCCESS)
		{			
			ap_log_perror(APLOG_MARK, APLOG_ERR, ret, server_conf->pool, "Failed loading application entry point: '%s'", application_entry_point);
			
			return HTTP_INTERNAL_SERVER_ERROR;
		}
		
		return APR_SUCCESS;
	}
	
	return HTTP_INTERNAL_SERVER_ERROR;
}

static int fenix_fixup(request_rec* apc_request)
{
	//TODO use some advanced logic to merge home folder with app filename and log filename
	fenix_dir_conf* dir_conf = (fenix_dir_conf*)ap_get_module_config(apc_request->per_dir_config, &fenix_module);
	fenix_server_conf* server_conf = (fenix_server_conf*)ap_get_module_config(apc_request->server->module_config, &fenix_module);
	
	
	char* app_filename = dir_conf->app_filename;
	
	if(app_filename && strcmp(app_filename, ""))
	{
		apr_status_t ret;
		
		apc_request->handler = apr_pstrdup(apc_request->pool, "fenix");
	
		char* home_folder = dir_conf->app_home_folder;
		char* dll_filename = dir_conf->app_filename;
		
		char* dll_full_name;
		shared_ptr<view::Response> (*_handle)(const action::Request&);
		
		apr_filepath_merge(&dll_full_name, home_folder, dll_filename, APR_FILEPATH_NATIVE, apc_request->pool);
		
		//Reload it every time or try to find it in cache
		/*if(dir_conf->app_reload)
		{*/
			ret = get_handle(&_handle, /*apc_request->pool,*/ dll_full_name, server_conf);
					
			if (ret != APR_SUCCESS) 
			{
				return HTTP_INTERNAL_SERVER_ERROR;
			}
		//}
		
		if(_handle)
		{			
			dir_conf->handle = _handle;
		}
	}

	return OK;
}


static void fenix_hooks(apr_pool_t* apc_pool)
{
	//ap_hook_child_init(fenix_init, NULL, NULL, APR_HOOK_MIDDLE);
	//ap_hook_post_config(fenix_post_config, NULL, NULL, APR_HOOK_MIDDLE) ;
	ap_hook_fixups(fenix_fixup, NULL, NULL, APR_HOOK_FIRST);
	ap_hook_handler(fenix_handler, NULL, NULL, APR_HOOK_MIDDLE);
	
}

extern "C" module AP_MODULE_DECLARE_DATA fenix_module =
{
	STANDARD20_MODULE_STUFF,
	fenix_create_dir_conf,
	fenix_merge_dir_conf,
	fenix_create_server_conf,
	/*fenix_merge_server_conf,*/
	NULL,
	fenix_directives,
	fenix_hooks
};
