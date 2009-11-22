#pragma once

#include <stdio.h>
#include <httpd.h>

struct fenix_dir_conf
{
	int app_reload;
	char* app_home_folder;
	char* app_filename;
	char* log_filename;

	FILE* log_file;
};

void* fenix_create_dir_conf(apr_pool_t* apc_pool, char* x);
void* fenix_merge_dir_conf(apr_pool_t* apc_pool, void* base_conf, void* add_conf);
