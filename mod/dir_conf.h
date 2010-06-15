#pragma once

#include <stdio.h>
#include <httpd.h>

#include <apr_dso.h>
#include <apr_tables.h>

#include "action.h"
#include "common.h"

using namespace fenix::web::toolkit;

struct fenix_dir_conf
{
	int 	app_reload;
	char* 	app_home_folder;
	char* 	app_filename;
	
	apr_table_t* app_config;
	
	//apr_dso_handle_t* module;
	shared_ptr<view::Response> (*handle)(const action::Request&);
};

void* fenix_create_dir_conf(apr_pool_t* apc_pool, char* x);
void* fenix_merge_dir_conf(apr_pool_t* apc_pool, void* base_conf, void* add_conf);
