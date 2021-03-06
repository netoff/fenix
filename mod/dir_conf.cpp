#include "dir_conf.h"

void* fenix_create_dir_conf(apr_pool_t* apc_pool, char* x)
{
	fenix_dir_conf* dir_conf = (fenix_dir_conf*)apr_pcalloc(apc_pool, sizeof(fenix_dir_conf));

	dir_conf->app_home_folder = NULL;
	dir_conf->app_filename = NULL;
	
	dir_conf->app_config = apr_table_make(apc_pool, 10);
	
	dir_conf->app_reload = 0;
	
	/*
	dir_conf->log_filename = NULL;*/

	return dir_conf;
}
void* fenix_merge_dir_conf(apr_pool_t* apc_pool, void* base_conf, void* add_conf)
{
	fenix_dir_conf* base = (fenix_dir_conf*)base_conf;
	fenix_dir_conf* add = (fenix_dir_conf*)add_conf;
	fenix_dir_conf* conf = (fenix_dir_conf*)apr_palloc(apc_pool, sizeof(fenix_dir_conf));

	conf->app_reload = (base->app_reload || add->app_reload)? 1 : 0;
	
	conf->app_home_folder =
		( add->app_home_folder == NULL ) ? base->app_home_folder : add->app_home_folder ;
	conf->app_filename =
		(add->app_filename == NULL) ? base->app_filename : add->app_filename;
		
	conf->app_config = apr_table_overlay(apc_pool, add->app_config, base->app_config);
		
	/*
	conf->log_filename =
		(add->log_filename == NULL ) ? base->log_filename : add->log_filename;*/

	return conf;
}
