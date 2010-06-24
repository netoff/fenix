#include "debug.h"
#include "dir_conf.h"
#include "request.h"
#include "cookie.h"
#include "mod_fenix.h"

#include <httpd.h>
#include <http_config.h>
#include <http_log.h>
#include <apr_uri.h>
#include <apr_strings.h>
#include <apr_buckets.h>
#include <apr_md5.h>

#include "boost/algorithm/string.hpp"

using namespace fenix::web::toolkit;

//POST max size 2MB
#define HTTP_POST_MAX_SIZE 2*1024*1024

//MACRO: returns empty string if char* is NULL
#define _S(X) ((X)?X:"")

string get_params_md5(const string& str)
{
	 unsigned char digest[APR_MD5_DIGESTSIZE];
	 apr_md5(digest, str.c_str(), strlen(str.c_str()));

	 static const char *hex = "0123456789abcdef";
	 char s[APR_MD5_DIGESTSIZE * 2 + 1];

	 int i;

	 for (i = 0; i < APR_MD5_DIGESTSIZE; i++)
	 {
		s[i*2]   = hex[digest[i] >> 4];
		s[i*2+1] = hex[digest[i] & 0x0f];
	 }
	 s[i*2] = '\0';

	 return s;
}

char* get_apc_post_data(request_rec* apc_request)
{
	int bytes;
	int eos = 0;
	apr_size_t count;
	apr_status_t rv;
	apr_bucket_brigade *bb;
	apr_bucket_brigade *bbin;

	apr_bucket *b;
	char* args;

	//This is not POST request
	string ctype = _S(apr_table_get(apc_request->headers_in, "Content-Type"));
	if (!boost::contains(ctype, "application/x-www-form-urlencoded") || apc_request->method_number != M_POST)
		return "";

	const char *clen = apr_table_get(apc_request->headers_in, "Content-Length");

	if (clen != NULL)
	{
		bytes = strtol(clen, NULL, 0);
		if (bytes >= HTTP_POST_MAX_SIZE)
		{
			return "";
		}
	}
	else
		bytes = HTTP_POST_MAX_SIZE;

	bb = apr_brigade_create(apc_request->pool, apc_request->connection->bucket_alloc);
	bbin = apr_brigade_create(apc_request->pool, apc_request->connection->bucket_alloc);

	do
	{
		rv = ap_get_brigade(apc_request->input_filters, bbin, AP_MODE_READBYTES, APR_BLOCK_READ, bytes);
		if (rv != APR_SUCCESS)
		{
			return "";
		}

		for (b = APR_BRIGADE_FIRST(bbin); b != APR_BRIGADE_SENTINEL(bbin); b = APR_BUCKET_NEXT(b))
		{
			if (APR_BUCKET_IS_EOS(b))
			{
				eos = 1;
				break;
			}

			if(!APR_BUCKET_IS_METADATA(b))
			{
				apr_bucket *ce;
				apr_bucket_copy(b, &ce);
				APR_BRIGADE_INSERT_TAIL(bb, ce);
			}
		}

	} while (!eos);

	APR_BRIGADE_INSERT_TAIL(bb, apr_bucket_eos_create(apc_request->connection->bucket_alloc));

	apr_brigade_pflatten(bb, &args, &count, apc_request->pool);
	args[count] = '\0';

	return args;
}
//Gets GET or POST params. Values should be url_undecoded and ready to use(tokenize..)
std::string get_apc_request_params(request_rec* apc_request)
{
	//TODO: should we first tokenize and then decode?? potential bug here??
	std::string ctype;
	char* args;
	char* p;
	
	switch(apc_request->method_number)
	{
	case M_GET:
		if(apc_request->args)
		{
			args = apr_pstrdup(apc_request->pool, apc_request->args);
		}
		else
		{
			args = "";
		}
		break;
	case M_POST:
		ctype = _S(apr_table_get(apc_request->headers_in, "Content-Type"));
		if (boost::contains(ctype, "application/x-www-form-urlencoded"))
		{
			//Pull the data out of request
			args = get_apc_post_data(apc_request);
		}
		else
		{
			args = "";
		}
		break;
	default:
		//We do not support anything else than GET/POST
		args = "";
	}

	for (p = args; *p; ++p)
	{
		//Replace pluses with spaces
		if (*p == '+')
			*p = ' ';
	}
	//ap_unescape_url(args);

	return args;
}

static int _get_note(void* rec, const char* key, const char* val)
{
	Request* request = (action::Request*)rec;
	request->setParam("config", key, val);
	
	return 1;
}

static int _get_config(void* rec, const char* key, const char* val)
{	
	Request* request = (action::Request*)rec;
	
	request->setParam("config", key, val);
	
	return 1;
}

void parse_notes(request_rec* apc_request, Request& request)
{
	apr_table_do(_get_note, (void*)&request, apc_request->notes, NULL);	
}

void parse_config(request_rec* apc_request, Request& request)
{
	fenix_dir_conf* dir_conf = (fenix_dir_conf*)ap_get_module_config(apc_request->per_dir_config, &fenix_module);
	
	apr_table_do(_get_config, (void*)&request, dir_conf->app_config, NULL);	
}

apr_status_t send_apc_file(string filename, string mime_type, request_rec* apc_request)
{
	const char *fname = filename.c_str();
	apr_file_t *fd;
	apr_size_t nbytes;
	apr_status_t rv;
	apr_finfo_t finfo;

    	rv=apr_stat(&finfo, fname, APR_FINFO_SIZE, apc_request->pool);
    	if (rv != APR_SUCCESS)
	{
		APC_ELOG(apc_request, "Error geting file stats for file: %s", fname);
		return HTTP_INTERNAL_SERVER_ERROR;
	}

    	rv=apr_file_open(&fd, fname, APR_READ, APR_OS_DEFAULT, apc_request->pool);
    	if (rv != APR_SUCCESS)
    	{
		APC_ELOG(apc_request, "Error opening file: %s", fname);
		return HTTP_INTERNAL_SERVER_ERROR;
    	}	

	ap_set_content_length(apc_request, finfo.size);
	ap_set_content_type(apc_request, mime_type.c_str());
	rv = ap_send_fd(fd, apc_request, 0, finfo.size, &nbytes);

	apr_file_close(fd);

	if(rv == APR_SUCCESS)
	{
		return rv;
	}
	else
	{
		return HTTP_INTERNAL_SERVER_ERROR;
	}
}

static string get_ip_addr(request_rec* apc_request)
{
	string ret = "";
	
	if (apr_table_get(apc_request->subprocess_env, "HTTP_CLIENT_IP")) 
	{
		ret = string(apr_pstrdup(apc_request->pool, 
			apr_table_get(apc_request->subprocess_env, "HTTP_CLIENT_IP")));
	}
	else if (apr_table_get(apc_request->subprocess_env, "HTTP_X_FORWARDED_FOR")) 
	{
		ret = string(apr_pstrdup(apc_request->pool,
			apr_table_get(apc_request->subprocess_env, "HTTP_X_FORWARDED_FOR")));
	}
	else if (apr_table_get(apc_request->headers_in, "X-Forwarded-For")) 
	{
		ret = string(apr_pstrdup(apc_request->pool,
				apr_table_get(apc_request->headers_in, "X-Forwarded-For")));
	}
	else if (apr_table_get(apc_request->subprocess_env, "HTTP_REMOTE_ADDR")) 
	{
		ret = string(apr_pstrdup(apc_request->pool, 
			apr_table_get(apc_request->subprocess_env, "HTTP_REMOTE_ADDR")));
	}
	else 
	{
		ret = string(apc_request->connection->remote_ip);
	}
	
	return ret;
}

void prepare_request(Request& request, request_rec* apc_request)
{
	switch(apc_request->method_number)
	{
	case M_GET:
		request._type = action::Request::READ;
		break;
	case M_POST:
		request._type = action::Request::WRITE;
		break;
	default:
		request._type = action::Request::UNKNOWN;
	}

	request._server_name = _S(ap_get_server_banner());

	fenix_dir_conf* dir_conf = (fenix_dir_conf*)ap_get_module_config(apc_request->per_dir_config, &fenix_module);	
	
	char* home_folder = dir_conf->app_home_folder;

	if(home_folder && strcmp(home_folder, ""))
	{
		request._home_path = home_folder;
	}
	else
	{
		request._home_path = ap_document_root(apc_request);
	}

	request._sparams["http"]["protocol"] =		_S(apc_request->protocol);
	request._sparams["http"]["uri"] =		_S(apc_request->uri);
	request._sparams["http"]["unparsed_uri"] = 	_S(apc_request->unparsed_uri);
	request._sparams["http"]["args"] =		_S(apc_request->args);
	request._sparams["http"]["path_info"] =		_S(apc_request->path_info);
	request._sparams["http"]["filename"] =		_S(apc_request->filename);
	request._sparams["http"]["port"] =		_S(apc_request->parsed_uri.port_str);
	request._sparams["http"]["scheme"] =		_S(ap_http_scheme(apc_request));
	string _h = 															_S(apc_request->hostname);
	request._sparams["http"]["hostname"] =		_h;
	request._sparams["http"]["remote_ip"] =		_S(apc_request->connection->remote_ip);
	request._sparams["http"]["remote_host"] =	_S(ap_get_remote_host(apc_request->connection, apc_request->per_dir_config, REMOTE_NAME, NULL));
	request._sparams["http"]["user_agent"] =	_S(apr_table_get(apc_request->headers_in, "User-Agent"));
	request._sparams["http"]["referer"] =		_S(apr_table_get(apc_request->headers_in, "Referer"));
	request._sparams["http"]["ajax"] =		_S(apr_table_get(apc_request->headers_in, "X-Requested-With"));

	//string arguments(_S(apc_request->args));
	string arguments = get_apc_request_params(apc_request);
	request._md5 = get_params_md5(arguments);

	typedef char_separator<char> ch_sep;
	typedef tokenizer<ch_sep> tokenizer;
	
	tokenizer parts(_h, ch_sep("."));
	
	foreach(string part, parts)
	{
		request._host.push_front(part);
	}

	ch_sep arg_sep("&");
	ch_sep keyval_sep("=");

	tokenizer args(arguments, arg_sep);

	foreach(string arg, args)
	{
		tokenizer kv(arg, keyval_sep);
		vector<string> kv_vec(kv.begin(), kv.end());

		string key = (kv_vec.size() > 0)?kv_vec[0]:"";
		string val = (kv_vec.size() > 1)?decode_param(kv_vec[1]):"";
		request._params[key] = val;
	}

	string url(_S(apc_request->uri));

	ch_sep uri_sep("/");
	tokenizer uric(url, uri_sep);

	foreach(string uri, uric)
	{
		request._query.push_back(uri);
	}		
	
	parse_notes(apc_request, request);
	parse_config(apc_request, request);
	
	//parse cookies
	char* c = apr_table_get(apc_request->headers_in, "Cookie");
	
	if(c)
	{
		string cookie(c);
		parse_cookies(cookie, request);
	}
	
	//IP address
	request._ip = get_ip_addr(apc_request);
}
