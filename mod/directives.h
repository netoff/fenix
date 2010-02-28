#pragma once

#include <http_config.h>
#include "dir_conf.h"

static const char* _set_conf(cmd_parms* cmd, void* cnf, const char* key, const char* val)
{
	fenix_dir_conf* dir_conf = (fenix_dir_conf*)cnf;
	
	if(!key || ! val)
	{
		return "You have to specify both config name and value!";
	}
	
	apr_table_set(dir_conf->app_config, key, val);
	
	return NULL;
}

static const command_rec fenix_directives[] =
{
	AP_INIT_FLAG("FenixAppReload",
			(const char* (*)())ap_set_flag_slot, (void*)APR_OFFSETOF(fenix_dir_conf, app_reload),
			ACCESS_CONF | OR_LIMIT | OR_OPTIONS | OR_FILEINFO | OR_AUTHCFG | OR_INDEXES,
			"Enable Application reload if there is newer version of app's dll."),

	AP_INIT_TAKE1("FenixAppFilename",
			(const char* (*)())ap_set_string_slot, (void*)APR_OFFSETOF(fenix_dir_conf, app_filename),
			ACCESS_CONF | OR_LIMIT | OR_OPTIONS | OR_FILEINFO | OR_AUTHCFG | OR_INDEXES,
			"Sets application's dll filename and path."),

	AP_INIT_TAKE1("FenixAppHome",
			(const char* (*)())ap_set_string_slot, (void*)APR_OFFSETOF(fenix_dir_conf, app_home_folder),
			ACCESS_CONF | OR_LIMIT | OR_OPTIONS | OR_FILEINFO | OR_AUTHCFG | OR_INDEXES,
			"Sets application's home folder."),
	
	AP_INIT_TAKE2("FenixConf",
			_set_conf, NULL,
			ACCESS_CONF | OR_LIMIT | OR_OPTIONS | OR_FILEINFO | OR_AUTHCFG | OR_INDEXES,
			"Sets application's custom config."),

	/*
	AP_INIT_TAKE1("FenixLogfile",
			(const char* (*)())ap_set_string_slot, (void*)APR_OFFSETOF(fenix_dir_conf, log_filename),
			ACCESS_CONF | OR_LIMIT | OR_OPTIONS | OR_FILEINFO | OR_AUTHCFG | OR_INDEXES,
			"Sets application log filename."),*/
	{ NULL }
};
