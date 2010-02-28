#include "common.h"

#include "mod_fenix.h"
#include "dir_conf.h"
#include "debug.h"

#include "handler.h"
#include "request.h"
#include "cookie.h"

#include <apr_strings.h>

using namespace std;
using namespace fenix::web::toolkit;

int _dispatch(const view::Response& response, request_rec* apc_request, const map<string, pair<string, int> >& session)
{
	try
	{
		string log_output = response.get_log_output();
		
		if(!log_output.empty())
		{
			ap_log_rerror("mod/handler.cpp", 0, APLOG_ERR, 0, apc_request, log_output.c_str());
		}
		
		//set cookies
		typedef pair<string, pair<string, int> > cookie_jar;		
		
		switch(response.getResponseType())
		{
		case view::Response::XML:
		case view::Response::JAVASCRIPT:
		case view::Response::HTML:
		case view::Response::PLAIN_TEXT:
		case view::Response::DHTML:
			{	
				
				foreach(cookie_jar c, session)
				{			
						string cookie_s = "";
												
						if(c.second.first.empty())
						{
							//clear cookie
							cookie_s = cookie(c.first, "", "/", -24);
						}
						else
						{	int expire = c.second.second;
							
							if(expire == 0)
							{
								//set new session cookie
								cookie_s = cookie(c.first, c.second.first, "/");
							}
							if(expire > 0)
							{
								//set new cookie
								cookie_s = cookie(c.first, c.second.first, "/", c.second.second);
							}						
						}
						
						if(!cookie_s.empty())
						{
							apr_table_add(apc_request->headers_out, "Set-Cookie", cookie_s.c_str());
						}
				}
			
				
				//TODO: Force mime-type based on response type class?? Or we do not need
				//cast to something. Response class should not be polymorphic at all??
				//Performance improvement, uniform API, no type conversion and casting....
				string mime_type = response.getMimeType() + "; charset=UTF-8";

				char* mtype = apr_pstrdup(apc_request->pool, mime_type.c_str());

				ap_set_content_type(apc_request, mtype);

				char* body = apr_pstrdup(apc_request->pool, response.getResponseBodyC());
				ap_rputs(body, apc_request);

				return OK;
			}
		case view::Response::NOT_FOUND:
			{
				char* mtype = "text/html";
				ap_set_content_type(apc_request, mtype);
				ap_rputs(response.getResponseBodyC(), apc_request);

				return HTTP_NOT_FOUND;
			}
		case view::Response::FORBIDEN:
			{
				char* mtype = "text/html";
				ap_set_content_type(apc_request, mtype);
				ap_rputs(response.getResponseBodyC(), apc_request);

				return HTTP_FORBIDDEN;
			}
		case view::Response::BAD_REQUEST:
			{
				char* mtype = "text/html";

				ap_set_content_type(apc_request, mtype);
				ap_rputs(response.getResponseBodyC(), apc_request);

				return HTTP_BAD_REQUEST;
			}
		case view::Response::REDIRECT:
			{
				foreach(cookie_jar c, session)
				{			
					string cookie_s = "";
											
					if(c.second.first.empty())
					{
						//clear cookie
						cookie_s = cookie(c.first, "", "/", -24);
					}
					else
					{	int expire = c.second.second;
						
						if(expire == 0)
						{
							//set new session cookie
							cookie_s = cookie(c.first, c.second.first, "/");
						}
						//-1 is cookie readed from request, it is already set, so do not have to set it again??
						if(expire > 0)
						{
							//set new cookie
							cookie_s = cookie(c.first, c.second.first, "/", c.second.second);
						}						
					}
					
					if(!cookie_s.empty())
					{
						apr_table_add(apc_request->err_headers_out, "Set-Cookie", cookie_s.c_str());
					}
				}
				
				apr_table_set(apc_request->headers_out, "Location", response.getResponseBodyC());
				
				return HTTP_MOVED_TEMPORARILY;  
			}
		case view::Response::MESSAGE:
		case view::Response::RESTRICT:
		case view::Response::EXCEPTION:
		case view::Response::JSON:
		case view::Response::AJAX:
		case view::Response::FILE:
			{
				string filename = response.getResponseBody();
				string mime_type = response.getMimeType();

				return send_apc_file(filename, mime_type, apc_request);
			}
		case view::Response::NEXT_ACTION:
		case view::Response::UNSUPPORTED:
		default:
			return HTTP_INTERNAL_SERVER_ERROR;
		}

	}//TODO: Why this does not catch ANY and EVERY exception at app level???
	catch(...)
	{
		return HTTP_INTERNAL_SERVER_ERROR;
	}
}

int fenix_handler(request_rec* apc_request)
{

	int rv = HTTP_INTERNAL_SERVER_ERROR;
	
	shared_ptr<view::Response> (*_handle)(const action::Request&) = NULL;

	fenix_dir_conf* dir_conf = (fenix_dir_conf*)ap_get_module_config(apc_request->per_dir_config, &fenix_module);

	char* home_folder = dir_conf->app_home_folder;
	char* dll_filename = dir_conf->app_filename;
	char* dll_full_name;

	apr_dso_handle_t* dll_module;

	if(!apc_request->handler || strcmp(apc_request->handler, "fenix"))
		return DECLINED;

	if (apc_request->method_number == M_OPTIONS)
	{
		apc_request->allowed |= (1 << M_GET);
		apc_request->allowed |= (1 << M_POST);
        return DECLINED;
	}

	if(apc_request->method_number != M_GET && apc_request->method_number != M_POST)
		return HTTP_METHOD_NOT_ALLOWED;


	apr_filepath_merge(&dll_full_name, home_folder, dll_filename, APR_FILEPATH_NATIVE, apc_request->pool);

	//TODO: Do not reload shared library on every request.
	//Slows down and waste resources...
	if(rv = apr_dso_load(&dll_module, dll_full_name, apc_request->pool))
	{
		char err[1024] = "";

		apr_dso_error(dll_module, err, sizeof(err));
		ap_log_rerror(APLOG_MARK, APLOG_ERR, rv, apc_request, "Failed loading shared library: %s(%s)", dll_full_name, err);
		
		return HTTP_INTERNAL_SERVER_ERROR;
	}
	if(rv = apr_dso_sym((void**)&_handle, dll_module, "handle"))
	{
		ap_log_rerror(APLOG_MARK, APLOG_ERR, rv, apc_request, "Failed loading module function: %s", "handle");

		apr_dso_unload(dll_module);
		
		return HTTP_INTERNAL_SERVER_ERROR;
	}

	action::Request request;

	prepare_request(request, apc_request);

	shared_ptr<view::Response> response(_handle(request));

	rv = _dispatch(*response, apc_request, request._pparams);
	
	return rv;
}
