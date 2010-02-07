#pragma once

#include <fenix.h>
#include "../model/db_conf.h"
#include "../model/tokyo.h"

const long session_duration = 60/*seconds*/ * 30/*minutes*/;

void log_page_view(string site_id, string url, string title, string referrer, string country_code, string search_query, long last_view, ptime timestamp)
{
	string key = str(format("s:%i")%site_id);
	string query = "";
	
	ostringstream sec_slot, min_slot, hour_slot, day_slot;
	
	string url_id = "";
	string title_id = "";
	string referrer_id = "";
	
	url_id = make_id(key + ":urls", url);
	title_id = make_id(key + ":titles", title);
	referrer_id = make_id(key + ":referrers", referrer);
	
	sec_slot << "hps";
	min_slot << "hpm";
	hour_slot << "hph";
	day_slot << "hpd";
	
	if(!url.empty())
	{		
		hour_slot << "|uph_" << url_id;
		day_slot << "|upd_" << url_id;
	}
	if(!title.empty())
	{
		
		hour_slot << "|tph_" << title_id;
		day_slot << "|tpd_" << title_id;
	}
	if(!referrer.empty())
	{
		hour_slot << "|rph_" << referrer_id;
		day_slot << "|rpd_" << referrer_id;
	}
	
	//------------------------------
	//new visitor
	if(last_view < 0)
	{
		sec_slot << "|nvps" ;
		min_slot << "|nvpm";
		
		hour_slot << "|sph|nvph" << "|scph_" << country_code;
		day_slot << "|spd|nvpd" << "|scpd_" << country_code
			<< "|scnpd_" << country_code;
		
		if(!url_id.empty())
		{
			day_slot << "|unpd_" << url_id;
		}
		if(!title_id.empty())
		{
			day_slot << "|tnpd_" << title_id;
		}
		if(!referrer_id.empty())
		{
			day_slot << "|rnpd_" << referrer_id;
		}
	}
	
	//-----------------------------
	//new session
	if(last_view > session_duration )
	{
		
		sec_slot << "|rvps";
		min_slot << "|rvpm";
		hour_slot << "|sph" << "|scph_" << country_code;
		day_slot << "|spd" << "|scpd_" << country_code;
		
		if(!url_id.empty())
		{
			day_slot << "|uspd_" << url_id;
		}
		if(!title_id.empty())
		{
			day_slot << "|tspd_" << title_id;
		}
		if(!referrer_id.empty())
		{
			day_slot << "|rspd_" << referrer_id;
		}
					
		ptime last_view_time = timestamp - posix_time::seconds(last_view);
		
		if(last_view_time.date() < timestamp.date())
		{
			//it was yesterday, or some days ago => returning visitor				
			day_slot << "|rvpd";				
		}
		else
		{
			//it is today => repeating visitor				
			day_slot << "|rpvpd";				
		}
	}
	
	string sec = sec_slot.str();
	string min = min_slot.str();
	string hour = hour_slot.str();
	string day = day_slot.str();
	
	query = make_query(sec, min, hour, day, timestamp);
	
	if(!query.empty())
	{
		model::Database(db_hostname, db_port).query("log", key, query);
	}
		
}
