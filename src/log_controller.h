#pragma once

#include <fenix.h>

namespace LogController
{
	const long session_duration = 60/*seconds*/ * 30/*minutes*/;
	
	FENIX_CONTROLLER(hit)
	{
		//get_params<string>
		string site_id = params["s"];

		//int timestamp = t & 0x03;

		string server_time = to_simple_string(request._timestamp);
		long timestamp = datetime::timestamp(request._timestamp);

		//Circular buffer with 8 elements
		long t = timestamp & 7;

		string key = str(format("s:%s")%site_id);
		string val = str(format("%i=vps:%i")%timestamp%t);

		model::Database db("localhost", 1978);
		string n = db.query("incr", key, val);

		string response = str(format("%s:%s")%t%n);

		return new InlineResponse(response, "text/plain");
	}
	
	void log_page_view(string site_id, string url, string title, string referrer, string country_code, long last_view, ptime timestamp)
	{
		string key = str(format("s:%i")%site_id);
		
		model::Database db("localhost", 1978);
		
		long seconds = datetime::timestamp(timestamp);
		long minutes = datetime::timestamp_m(timestamp);
		long hours = datetime::timestamp_h(timestamp);
		long days = datetime::timestamp_d(timestamp);
		
		if(last_view < 0)
		{
			//new visitor
			string query = str(format("%i=hps|nvps/%i=hpm|nvpm/%i=sph/%i=hpd|nvpd")%seconds %minutes %hours %days);
			
			db.query("log", key, query);
		}
		else if(last_view <= session_duration )
		{
			//same session
			string query = str(format("%i=hps/%i=hpm/ /%i=hpd")%seconds %minutes %days);
			
			db.query("log", key, query);
		}
		else 
		{
			//new session
			ptime last_view_time = timestamp - posix_time::seconds(last_view);
			
			if(last_view_time.date() < timestamp.date())
			{
				//it was yesterday, or some days ago => returning visitor
				string query = str(format("%i=hps|rvps/%i=hpm|rvpm/%i=sph/%i=hpd|rvpd")%seconds %minutes %hours %days);
				
				db.query("log", key, query);				
			}
			else
			{
				//it is today => repeating visitor
				string query = str(format("%i=hps|rvps/%i=hpm|rvpm/%i=sph/%i=hpd|rpvpd")%seconds %minutes %hours %days);
				
				db.query("log", key, query);				
			}
		}
			
	}
	
	void log_click()
	{
	}
	
	void log_event()
	{
	}
	
	void log_permalink_click()
	{
	}
	
	FENIX_CONTROLLER(lg)
	{
		string site_id;
		string event_type;
		
		if(
			get_param(params["id"], site_id) && 
			get_param(params["e"], event_type) )
		{
			if(event_type == "view")
			{
				string url, title, referrer, country_code;
				long last_view; //seconds ago
				
				get_param(params["u"], url);
				get_param(params["t"], title);
				get_param(params["r"], referrer);
				get_param(params["tm"], last_view);
				
				country_code = "0";//temp, get real country code
				
				log_page_view(site_id, url, title, referrer, country_code, last_view, request._timestamp);
			}
			if(event_type == "click")
			{
				log_click();
			}
			if(event_type == "event")
			{
				log_event();
			}
			
			
			string response = "view log";
			return new InlineResponse(response, "text/plain");
			
			//return render_<TrackingPixel>();
		}
		else
		{
			//return render_<BadRequest>();
			//return render_<EtcRequest>(param1, param2, param3...);
			//return render_tracking_pixel();
			//return render_text("Text Text");
			//return render_redirect("/file1/file2/file3");
			//
			//would create smart ptr, and return it...
			
			return render_<BadRequestResponse>();
		}
	}
}
