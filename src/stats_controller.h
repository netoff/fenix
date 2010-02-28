#pragma once

#include "fenix.h"
#include "views.h"

using namespace boost;

using namespace fenix::web::toolkit;
using namespace pages;

namespace StatsController
{
	string get_stats_per_second(const model::Database& db, const string& key, const long timestamp1, const long timestamp2)
	{
		string query =  "['get_values', {'keys': ['hps', 'nvps', 'rvps'], 'max': 1, 'from': %i, 'timestamp': %i, 'slot': 'sec', 'encode': true}]";
		string data = db.query(key, str(format(query)%timestamp1 %timestamp2));
		
		return data;
	}
	
	string get_stats_per_minute(const model::Database& db, const string& key, const long timestamp1, const long timestamp2)
	{
		string query = "['get_values', {'keys': ['hpm', 'nvpm', 'rvpm'], 'max': 1, 'from': %i, 'timestamp': %i, 'slot': 'min', 'encode': true}]";
		string data = db.query(key, str(format(query)%timestamp1 %timestamp2));
		
		return data;		
	}
	
	string get_countries_list(const model::Database& db, const string& key, const long timestamp)
	{
		string query = "['get_list', {'key': 'scph', 'lookup': false, 'count': 24, 'max': 10, 'timestamp': %i, 'slot': 'hour', 'encode': true}]";
		string data = db.query(key, str(format(query)%timestamp));
		
		return data;
	}
	
	string get_titles_list(const model::Database& db, const string& key, const long timestamp)
	{
		string query = "['get_list', {'key': 'tph', 'lookup': 'titles', 'count':24, 'max': 10, 'timestamp': %i, 'slot': 'hour', 'encode': true}]";
		string data = db.query(key, str(format(query)%timestamp));
		
		return data;		
	}
	
	string get_referrers_list(const model::Database& db, const string& key, const long timestamp)
	{
		string query = "['get_list', {'key': 'rph', 'lookup': 'referrers', 'count': 24, 'max': 10, 'timestamp': %i, 'slot': 'hour', 'encode': true}]";
		string data = db.query(key, str(format(query)%timestamp));
		
		return data;
	}
	
	string get_search_queries_list(const model::Database& db, const string& key, const long timestamp)
	{
		string query = "['get_list', {'key': 'qph', 'lookup': 'queries', 'count': 24, 'max':10, 'timestamp': %i, 'slot': 'hour', 'encode': true}]";
		string data = db.query(key, str(format(query)%timestamp));
		
		return data;
	}
	
	FENIX_CONTROLLER(views)
	{
		string site_id;
		
		long t = 0;
		long t_m = 0;
	
		if(	get_param(request["id"], site_id) && 
			get_param(request["t"], t) &&
			get_param(request["t1"], t_m))
		{
			ostringstream response;
		
			long timestamp = datetime::timestamp(request._timestamp);
			long timestamp_m = datetime::timestamp_m(request._timestamp);
			long timestamp_h = datetime::timestamp_h(request._timestamp);
			
			if(t == 0) 
			{
				t = (timestamp > 60 ? timestamp - 60 : 1);
			}
			if(t_m == 0) 
			{
				t_m = (timestamp_m > 60 ? timestamp_m - 60 : 1);
			}
			
			string key = str(format("s:%s")%site_id);
			
			model::Database db(get_log_db(request).host, get_log_db(request).port);

			if(t < timestamp)
			{
				string points = get_stats_per_second(db, key, t, timestamp - 1);
				
				response << "dashboard.chart.updatePoints(" << points << ");\n";
				response << "dashboard.timestamp = " << timestamp << ";\n";
				response << "dashboard.chart.draw();\n";
				response << "dashboard.updateData();\n";
					
			}
			if(t_m < timestamp_m)
			{
				string points_m = get_stats_per_minute(db, key, t_m, timestamp_m - 1 );
				
				response << "dashboard.chartMin.updatePoints(" << points_m << ");\n";
				response << "dashboard.timestampMin = " << timestamp_m << ";\n";
				response << "dashboard.chartMin.draw();\n";
				response << "dashboard.updateDataMin();\n";
				
				string countries = get_countries_list(db, key, timestamp_h);
				string pages = get_titles_list(db, key, timestamp_h);
				string referrers = get_referrers_list(db, key, timestamp_h);
				string queries = get_search_queries_list(db, key, timestamp_h);
				
				response << "dashboard.updateCountriesList(" << countries << ");\n";
				response << "dashboard.updatePagesList(" << pages << ");\n";
				response << "dashboard.updateReferrersList(" << referrers << ");\n";
				response << "dashboard.updateQueriesList(" << queries << ");\n";
				
				response << "dashboard.updateClock('" << datetime::format(request._timestamp, "%H:%M %d-%b-%Y GMT") << "');\n";
				
				//response << "dashboard.adjustLayout();\n";
				
			}
			
			response << "setTimeout('dashboard.update()', 500);";
			
			return render_text(response, "text/javascript");
		}
		
		return render_<BadRequestResponse>();
	}
}
