#include "common.h"

#include "mod_fenix.h"
#include "dir_conf.h"
#include "debug.h"

#include "handler.h"
#include "request.h"
#include "cookie.h"

#include <apr_strings.h>
#include <apr_base64.h>

using namespace std;
using namespace fenix::web::toolkit;

typedef const map<string, action::cookie_param> Session;
typedef pair<string, action::cookie_param> cookie_jar;

static vector<string> get_cookies(Session& session)
{
	vector<string> ret;
	
	foreach(cookie_jar c, session)
	{			
		string cookie_val = "";
								
		if(c.second.val.empty())
		{
			//clear cookie
			if(c.second.domain.empty())
			{
				cookie_val = cookie(c.first, c.second.val, c.second.path, -24);
			}
			else
			{
				cookie_val = cookie(c.first, c.second.val, c.second.path, c.second.domain, -24);
			}
		}
		else
		{	
			int expire = c.second.expire;
			
			if(expire == 0)
			{
				//set new session cookie
				if(c.second.domain.empty())
				{
					cookie_val = cookie(c.first, c.second.val, c.second.path);
				}
				else
				{
					cookie_val = cookie(c.first, c.second.val, c.second.path, c.second.domain);
				}
			}
			if(expire > 0)
			{
				//set new cookie
				if(c.second.domain.empty())
				{
					cookie_val = cookie(c.first, c.second.val, c.second.path, c.second.expire);
				}
				else
				{
					cookie_val = cookie(c.first, c.second.val, c.second.path, c.second.domain, c.second.expire);
				}									
			}						
		}
		
		ret.push_back(cookie_val);
	}	
	
	return ret;
}

int _dispatch(const shared_ptr<view::Response> response, request_rec* apc_request, Session& session)
{
	try
	{	
		switch(response->getResponseType())
		{
		case view::Response::XML:
		case view::Response::JAVASCRIPT:
		case view::Response::HTML:
		case view::Response::PLAIN_TEXT:
		case view::Response::DHTML:
			{	

				vector<string> cookies = get_cookies(session);
				
				foreach(string cookie, cookies)
				{						
						if(!cookie.empty())
						{
							apr_table_add(apc_request->headers_out, "Set-Cookie", cookie.c_str());
						}
				}			
				
				//TODO: Force mime-type based on response type class?? Or we do not need
				//cast to something. Response class should not be polymorphic at all??
				//Performance improvement, uniform API, no type conversion and casting....
				string mime_type = response->getMimeType() + "; charset=UTF-8";

				char* mtype = apr_pstrdup(apc_request->pool, mime_type.c_str());

				ap_set_content_type(apc_request, mtype);

				char* body = apr_pstrdup(apc_request->pool, response->getResponseBodyC());
				ap_rputs(body, apc_request);

				return OK;
			}
		case view::Response::BINARY:
			{
				string mime_type = response->getMimeType();

				char* mtype = apr_pstrdup(apc_request->pool, mime_type.c_str());
				char* body = apr_pstrdup(apc_request->pool, response->getResponseBodyC());
				
				ap_set_content_type(apc_request, mtype);
				
				unsigned char* binary = (unsigned char*)apr_palloc(apc_request->pool, apr_base64_decode_len(body) + 1);
				int len = apr_base64_decode_binary(binary, body);
				binary[len] = '\0';//just in case
				
				//ap_rputs(binary, apc_request);
				ap_rwrite((void*)binary, len, apc_request);
								
				return OK;
			}
		case view::Response::NOT_FOUND:
			{
				char* mtype = "text/html";
				ap_set_content_type(apc_request, mtype);
				ap_rputs(response->getResponseBodyC(), apc_request);

				return HTTP_NOT_FOUND;
			}
		case view::Response::FORBIDEN:
			{
				char* mtype = "text/html";
				ap_set_content_type(apc_request, mtype);
				ap_rputs(response->getResponseBodyC(), apc_request);

				return HTTP_FORBIDDEN;
			}
		case view::Response::BAD_REQUEST:
			{
				char* mtype = "text/html";

				ap_set_content_type(apc_request, mtype);
				ap_rputs(response->getResponseBodyC(), apc_request);

				return HTTP_BAD_REQUEST;
			}
		case view::Response::REDIRECT:
			{
				vector<string> cookies = get_cookies(session);
				
				foreach(string cookie, cookies)
				{								
					if(!cookie.empty())
					{
						apr_table_add(apc_request->err_headers_out, "Set-Cookie", cookie.c_str());
					}
				}
				
				apr_table_set(apc_request->headers_out, "Location", response->getResponseBodyC());
				
				return HTTP_MOVED_TEMPORARILY;  
			}
		case view::Response::MESSAGE:
		case view::Response::RESTRICT:
		case view::Response::EXCEPTION:
		case view::Response::JSON:
		case view::Response::AJAX:
		case view::Response::FILE:
			{
				string filename = response->getResponseBody();
				string mime_type = response->getMimeType();

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
	int ret = HTTP_INTERNAL_SERVER_ERROR;	

	if(!apc_request->handler || strcmp(apc_request->handler, "fenix"))
	{
		return DECLINED;
	}

	if (apc_request->method_number == M_OPTIONS)
	{
		apc_request->allowed |= (1 << M_GET);
		apc_request->allowed |= (1 << M_POST);
		
		return DECLINED;
	}

	if(apc_request->method_number != M_GET && apc_request->method_number != M_POST)
	{
		return HTTP_METHOD_NOT_ALLOWED;
	}

	fenix_dir_conf* dir_conf = (fenix_dir_conf*)ap_get_module_config(apc_request->per_dir_config, &fenix_module);
	
	/*if(!dir_conf->module)
	{
		return HTTP_INTERNAL_SERVER_ERROR;
	}*/
	
	if(!dir_conf->handle)
	{
		return HTTP_INTERNAL_SERVER_ERROR;
	}
	
	shared_ptr<view::Response> (*_handle)(const action::Request&) = dir_conf->handle;
	/*
	ret = apr_dso_sym((void**)&_handle, dir_conf->module, application_entry_point);
	
	if(ret)
	{
		ap_log_rerror(APLOG_MARK, APLOG_ERR, ret, apc_request, "Failed loading application entry point: '%s'", application_entry_point);
		
		return HTTP_INTERNAL_SERVER_ERROR;
	}*/

	action::Request request;

	prepare_request(request, apc_request);

	shared_ptr<view::Response> response = _handle(request);

	ret = _dispatch(response, apc_request, request._pparams);
	
	return ret;
}
