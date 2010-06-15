#pragma once

#include <fenix.h>

#include "log_request.h"

#include "../model/db_conf.h"

const long session_duration = 60/*seconds*/ * 30/*minutes*/;

typedef ostringstream os;	

static string log_new_visitor(const string& ts, const string& req)
{
	os ret; ret << "log.log_new_visitor(" <<  ts << ", "<< req << ")";
	return ret.str();
}
static string log_returning_visitor(const string& ts, const string& req)
{
	os ret; ret << "log.log_returning_visitor(" << ts << ", " << req << ")";
	return ret.str();
}
static string log_repeating_visitor(const string& ts, const string& req)
{
	os ret; ret << "log.log_repeating_visitor(" << ts << ", " << req << ")";
	return ret.str();
}
static string log_page_view(const string& ts, const string& req)
{
	os ret; ret << "log.log_page_view(" << ts << ", " << req << ")";
	return ret.str();
}

void log_page_view(const LogRequest& req, long last_view, ptime timestamp, const ScopedMiddleware& mid)
{
	ostringstream query;
	
	string ts_obj = ts_object(timestamp);
	string req_obj = req.to_object();
	
	//new visitor
	if(last_view < 0)
	{
		query << log_new_visitor(ts_obj, req_obj);
	
	} 	
	//-----------------------------
	//new session
	else if(last_view > session_duration )
	{
		ptime last_view_time = timestamp - posix_time::seconds(last_view);
		
		if(last_view_time.date() < timestamp.date())
		{
			query << log_returning_visitor(ts_obj, req_obj);
		}
		else
		{
			query << log_repeating_visitor(ts_obj, req_obj);
		}
	}
	else
	{
		query << log_page_view(ts_obj, req_obj);
	}

	string q = query.str();
	
	if(!q.empty())
	{
		mid.insert(q);
	}
		
}
