#pragma once

#include "fenix.h"
#include "views.h"
#include "middleware.h"

#include "model/db_conf.h"

using namespace boost;

using namespace fenix::web::toolkit;
using namespace pages;

static const microsecond = 1000*1000;

namespace StatsController
{
	static string get_stats_per_second(const ScopedMiddleware& mid, const string& key, 
		const long timestamp1, const long timestamp2)
	{
		ostringstream query;
		
		query << "return dashboard.get_stats_per_sec({id = [[" << key << "]], ts1 = " 
			<< timestamp1 << ", ts2 = " << timestamp2 << "})";
		
		string data = mid.query(query.str());
		
		return data;
	}
	
	static string get_stats_per_minute(const ScopedMiddleware& mid, const string& key, 
		const long timestamp1, const long timestamp2)
	{
		ostringstream query;
		
		query << "return dashboard.get_stats_per_min({id = [[" << key << "]], ts1 = " 
			<< timestamp1 << ", ts2= " << timestamp2 << "})";
		
		string data = mid.query(query.str());
		
		return data;		
	}
	
	static string get_stats_per_hour(const ScopedMiddleware& mid, const string& key, 
		const long timestamp1, const long timestamp2)
	{
		ostringstream query;
		query << "return dashboard.get_stats_per_hour({id = [[" << key << "]], ts1 = " 
			<< timestamp1 << ", ts2 = " << timestamp2 << "})";
			
		string data = mid.query(query.str());
		
		return data;
	}
	
	static string get_dashboard_feed(const ScopedMiddleware& mid, const string& key, 
		const long timestamp_h, const long timestamp_d, const long long timestamp_micro)
	{
		ostringstream query;
		
		query << "return dashboard.get_feed({id = [[" << key << "]], ts = " 
				<< timestamp_h << ", ts_d = " << timestamp_d << ", ts_micro = " << timestamp_micro << "})";
		
		return mid.query(query.str());
	}
	
	FENIX_CONTROLLER(views)
	{
		string site_id;
		
		long t = 0;
		long t_m = 0;
		long t_h = 0;
		long long t_micro = 0;
	
		if( request.isRead() &&	
			get_param(request["id"], site_id) && 
			get_param(request["t"], t) &&
			get_param(request["t1"], t_m) &&
			get_param(request["t2"], t_h) &&
			get_param(request["t0"], t_micro))
		{
			if(this->_authenticate.authorize_user(site_id))
			{
				ostringstream response;
			
				long timestamp = datetime::timestamp(request._timestamp);
				long timestamp_m = datetime::timestamp_m(request._timestamp);
				long timestamp_h = datetime::timestamp_h(request._timestamp);
				long timestamp_d = datetime::timestamp_d(request._timestamp);
				long long timestamp_micro = datetime::timestamp_micro(request._timestamp);
				
				if(t == 0) t = (timestamp > 60 ? timestamp - 60 : 1);
				if(t_m == 0) t_m = (timestamp_m > 60 ? timestamp_m - 60 : 1);
				if(t_h == 0) t_h = (timestamp_h > 24 ? timestamp_h - 24: 1);
				if(t_micro == 0) t_micro = timestamp_micro - 30 * microsecond;
				
				//Do not send us false timestamps
				if(t <= timestamp && (timestamp - t) <= 60 &&
					t_m <= timestamp_m && (timestamp_m - t_m) <= 60 &&
					t_h <= timestamp_h && (timestamp_h - t_h) <= 24 &&
					t_micro <= timestamp_micro)
				{							
					ScopedMiddleware mid;	
					
					if(t < timestamp)
					{
						response << "dashboard.updateData(" 
							<< get_stats_per_second(mid, site_id, t, timestamp - 1) << ", " << timestamp << ");\n";						
					}				
					
					if(t_m < timestamp_m)
					{
						response << "dashboard.updateDataMin(" 
							<< get_stats_per_minute(mid, site_id, t_m, timestamp_m - 1 ) << ", " << timestamp_m << ");\n";
					}
					
					if(t_h < timestamp_h)
					{
						response << "dashboard.updateDataHour(" 
							<< get_stats_per_hour(mid, site_id, t_h, timestamp_h - 1) 
								<< ", " << timestamp_h << ");\n";
					}
					
					//update feed
					response 
						<< "dashboard.updateFromFeed(" 
							<< get_dashboard_feed(mid, site_id, timestamp_h, timestamp_d, t_micro) << ");\n";
					
					mid.done();
					
					return render_text(response, "text/javascript");
				}
			}
		}
		
		return render_<BadRequestResponse>();
	}
}
