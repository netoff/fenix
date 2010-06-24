#pragma once

#include <httpd.h>
#include <http_log.h>


inline void printrequest (request_rec * r)
{
	ap_log_rerror (__FILE__, __LINE__, APLOG_DEBUG, 0, r,
     "fenix: request_rec->the_request is %s", r->the_request);
  ap_log_rerror (__FILE__, __LINE__, APLOG_DEBUG, 0, r,
     "fenix: request_rec->protocol is %s", r->protocol);
  ap_log_rerror (__FILE__, __LINE__, APLOG_DEBUG, 0, r,
     "fenix: request_rec->hostname is %s", r->hostname);
  ap_log_rerror (__FILE__, __LINE__, APLOG_DEBUG, 0, r,
     "fenix: request_rec->status_line is %s", r->status_line);
  ap_log_rerror (__FILE__, __LINE__, APLOG_DEBUG, 0, r,
     "fenix: request_rec->method is %s", r->method);
  ap_log_rerror (__FILE__, __LINE__, APLOG_DEBUG, 0, r,
     "fenix: request_rec->range is %s", r->range);
  ap_log_rerror (__FILE__, __LINE__, APLOG_DEBUG, 0, r,
     "fenix: request_rec->content_type is %s", r->content_type);
  ap_log_rerror (__FILE__, __LINE__, APLOG_DEBUG, 0, r,
     "fenix: request_rec->handler is %s", r->handler);
  ap_log_rerror (__FILE__, __LINE__, APLOG_DEBUG, 0, r,
     "fenix: request_rec->content_encoding is %s",
     r->content_encoding);
  ap_log_rerror (__FILE__, __LINE__, APLOG_DEBUG, 0, r,
     "fenix: request_rec->content_languages is %s",
     r->content_languages);
  ap_log_rerror (__FILE__, __LINE__, APLOG_DEBUG, 0, r,
     "fenix: request_rec->vlist_validator is %s",
     r->vlist_validator);
  ap_log_rerror (__FILE__, __LINE__, APLOG_DEBUG, 0, r,
     "fenix: request_rec->user is %s", r->user);
  ap_log_rerror (__FILE__, __LINE__, APLOG_DEBUG, 0, r,
     "fenix: request_rec->ap_auth_type is %s", r->ap_auth_type);
  ap_log_rerror (__FILE__, __LINE__, APLOG_DEBUG, 0, r,
     "fenix: request_rec->unparsed_uri is %s", r->unparsed_uri);
  ap_log_rerror (__FILE__, __LINE__, APLOG_DEBUG, 0, r,
     "fenix: request_rec->uri is %s", r->uri);
  ap_log_rerror (__FILE__, __LINE__, APLOG_DEBUG, 0, r,
     "fenix: request_rec->filename is %s", r->filename);
  ap_log_rerror (__FILE__, __LINE__, APLOG_DEBUG, 0, r,
     "fenix: request_rec->path_info is %s", r->path_info);
  ap_log_rerror (__FILE__, __LINE__, APLOG_DEBUG, 0, r,
     "fenix: request_rec->args is %s", r->args);
  return;
}

#define APC_ELOG(req, msg, ...) ap_log_error(APLOG_MARK, APLOG_ERR,\
	0, req->server, msg, __VA_ARGS__)
#define APC_DLOG(req, msg, ...) ap_log_error(APLOG_MARK, APLOG_DEBUG,\
	0, req->server, msg, __VA_ARGS__)
