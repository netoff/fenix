#include "mod_fenix.h"

#include <apr_dso.h>
#include <apr_file_info.h>
#include <http_log.h>

#include "debug.h"
#include "dir_conf.h"
#include "directives.h"
#include "driver.h"
#include "handler.h"

static int fenix_fixup(request_rec* apc_request)
{
	//TODO use some advanced logic to merge home folder with app filename and log filename
	fenix_dir_conf* dir_conf = (fenix_dir_conf*)ap_get_module_config(apc_request->per_dir_config, &fenix_module);
	char* app_filename = dir_conf->app_filename;
	char* log_filename = dir_conf->log_filename;

	if(app_filename && strcmp(app_filename, ""))
	{
		apc_request->handler = "fenix";
	}
	//TODO do not open log file on evry request. keep this in server config.
	if(log_filename && strcmp(log_filename, ""))
	{
		dir_conf->log_file = fopen(log_filename, "a");
		if(dir_conf->log_file)
			apr_pool_cleanup_register(apc_request->pool, dir_conf->log_file, (apr_status_t (*)(void*))fclose, apr_pool_cleanup_null) ;
	}

	return OK;
}


static void fenix_hooks(apr_pool_t* apc_pool)
{
	//ap_hook_child_init(fenix_init, NULL, NULL, APR_HOOK_MIDDLE);
	ap_hook_fixups(fenix_fixup, NULL, NULL, APR_HOOK_FIRST);
	ap_hook_handler(fenix_handler, NULL, NULL, APR_HOOK_MIDDLE);
	
}

extern "C" module AP_MODULE_DECLARE_DATA fenix_module =
{
	STANDARD20_MODULE_STUFF,
	fenix_create_dir_conf,
	fenix_merge_dir_conf,
	/*fenix_create_server_conf,
	fenix_merge_server_conf,*/
	NULL,
	NULL,
	fenix_directives,
	fenix_hooks
};
