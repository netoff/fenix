#pragma once

#include "fenix.h"
#include "views.h"

#include <ctime>
#include <boost/random.hpp>

using namespace boost;

using namespace fenix::web::toolkit;
using namespace pages;

namespace StatsController
{
	string get_stats_per_second(const model::Database& db, const string& key, long timestamp)
	{
		string hps = db.query("get_slot_sec", key, str(format("%i=hps")%timestamp));
		string nvps = db.query("get_slot_sec", key, str(format("%i=nvps")%timestamp));
		string rvps = db.query("get_slot_sec", key, str(format("%i=rvps")%timestamp));
		
		return str(format("[%s, %s, %s]")%hps %nvps %rvps);
	}
	
	string get_stats_per_minute(const model::Database& db, const string& key, long timestamp)
	{
		string hpm = db.query("get_slot_min", key, str(format("%i=hpm")%timestamp));
		string nvpm = db.query("get_slot_min", key, str(format("%i=nvpm")%timestamp));
		string rvpm = db.query("get_slot_min", key, str(format("%i=rvpm")%timestamp));
		
		return str(format("[%s, %s, %s]")%hpm %nvpm %rvpm);		
	}
	
	string get_countries(const model::Database& db, const string& key, long timestamp)
	{
		string countries = db.query("get_last_24h", key, str(format("%i=scph")%timestamp));
		
		return string;
	}
	
	FENIX_CONTROLLER(views)
	{
		string site_id;
		
		long t = 0;
		long t_m = 0;
	
		if(	get_param(params["id"], site_id) && 
			get_param(params["t"], t) &&
			get_param(params["t1"], t_m))
		{
			ostringstream response;
		
			long timestamp = datetime::timestamp(request._timestamp);
			long timestamp_m = datetime::timestamp_m(request._timestamp);
			long timestamp_h = datetime::timestamp_h(request._timestamp);
			
			if(t == 0) t = (timestamp > 59 ? timestamp - 59 : 1);
			if(t_m == 0) t_m = (timestamp_m > 59 ? timestamp_m - 59 : 1);
			
			string key = str(format("s:%s")%site_id);
			
			model::Database db("localhost", 1978);

			if(t < timestamp)
			{
				string points = "";
				
				points += get_stats_per_second(db, key, t);

				for(++t; t < timestamp; t++)
				{					
					points += ", " + get_stats_per_second(db, key, t);
				}
				
				response << "dashboard.chart.updatePoints([" << points << "]);\n";
				response << "dashboard.timestamp = " << timestamp << ";\n";
				response << "dashboard.chart.draw();\n";
				response << "dashboard.updateData();\n";
					
			}
			if(t_m < timestamp_m)
			{
				string points_m = "";
				
				points_m += get_stats_per_minute(db, key, t_m);
				
				for(++t_m; t_m < timestamp_m; t_m++)
				{
					points_m += ", " + get_stats_per_minute(db, key, t_m);
				}
				
				response << "dashboard.chartMin.updatePoints([" << points_m << "]);\n";
				response << "dashboard.timestampMin = " << timestamp_m << ";\n";
				response << "dashboard.chartMin.draw();\n";
				response << "dashboard.updateDataMin();\n";
				
				string countries = get_countries(db, key, timestamp_h);
				
				response << "dashboard.updateCountries(" << countries << ");\n";
				
			}
			
			response << "setTimeout('dashboard.update()', 500);";
			
			return new InlineResponse(response, "text/javascript");
			//return render_text(response, "text/javascript");
		}
		else
		{
			return new BadRequestResponse();
			//return render_<BadRequestResponse>();
		}
	}
}
