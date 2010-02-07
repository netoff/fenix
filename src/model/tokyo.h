#pragma once

#include <fenix.h>
#include "db_conf.h"

string make_slot(long timestamp, string& s)
{
	ostringstream ret;
	
	if(!s.empty())
	{
		ret << timestamp << "=" << s;
	}
	
	return ret.str();		
}

string make_query(string& sec, string& min, string& hour, string& day, ptime timestamp)
{
	long seconds_t = datetime::timestamp(timestamp);
	long minutes_t = datetime::timestamp_m(timestamp);
	long hours_t = datetime::timestamp_h(timestamp);
	long days_t = datetime::timestamp_d(timestamp);
	
	ostringstream ret;
	
	ret << make_slot(seconds_t, sec) << "/" << make_slot(minutes_t, min) 
		<< "/" << make_slot(hours_t, hour) << "/" << make_slot(days_t, day);
		
	return ret.str();
}

string make_id(const string& key, const string& val)
{
	string ret = "";
	
	ret = model::Database(db_hostname, db_port).query("make_id", key, val);
	
	return ret;
}
