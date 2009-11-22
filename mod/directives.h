#pragma once

#include <http_config.h>

static const command_rec fenix_directives[] =
{
	AP_INIT_FLAG("FenixAppReload",
			(const char* (*)())ap_set_flag_slot, (void*)APR_OFFSETOF(fenix_dir_conf, app_reload),
			ACCESS_CONF | OR_LIMIT | OR_OPTIONS | OR_FILEINFO | OR_AUTHCFG | OR_INDEXES,
			"Enable Application reload if there is newer version of app's dll."),

	AP_INIT_TAKE1("FenixAppFilename",
			(const char* (*)())ap_set_string_slot, (void*)APR_OFFSETOF(fenix_dir_conf, app_filename),
			ACCESS_CONF | OR_LIMIT | OR_OPTIONS | OR_FILEINFO | OR_AUTHCFG | OR_INDEXES,
			"Sets application dll filename and path."),

	AP_INIT_TAKE1("FenixAppHome",
			(const char* (*)())ap_set_string_slot, (void*)APR_OFFSETOF(fenix_dir_conf, app_home_folder),
			ACCESS_CONF | OR_LIMIT | OR_OPTIONS | OR_FILEINFO | OR_AUTHCFG | OR_INDEXES,
			"Sets application home folder."),

	AP_INIT_TAKE1("FenixLogfile",
			(const char* (*)())ap_set_string_slot, (void*)APR_OFFSETOF(fenix_dir_conf, log_filename),
			ACCESS_CONF | OR_LIMIT | OR_OPTIONS | OR_FILEINFO | OR_AUTHCFG | OR_INDEXES,
			"Sets application log filename."),
	{ NULL }
};
