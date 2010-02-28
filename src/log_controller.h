#pragma once

#include <fenix.h>

#include "log/view.h"
#include "log/click.h"
#include "log/event.h"

#include "model/db_conf.h"

using namespace fenix::web::toolkit;

namespace LogController
{
	
	
	FENIX_CONTROLLER(lg)
	{		
		string site_id;
		string event_type;
		
		if(
			get_param(request["_id"], site_id) && 
			get_param(request["_e"], event_type) )
		{
			if(event_type == "hit")
			{			
				string url, title, referrer, country_code, search_query;
				long last_view; //seconds ago
				
				get_param(request["_u"], url);
				get_param(request["_t"], title);
				get_param(request["_r"], referrer);
				get_param(request["_tm"], last_view);
				get_param(request["_c"], country_code);
				
				DB db = get_log_db(request);
				
				log_page_view(site_id, url, title, referrer, country_code, search_query, last_view, request._timestamp, db);
			}
			if(event_type == "cl")
			{
				log_click();
			}
			if(event_type == "ev")
			{
				log_event();
			}
			
			
			string response = "view log";
			
			return render_text(response, "text/plain");
			
			//return render_<TrackingPixel>();
		}
		
		return render_<BadRequestResponse>();		
	}
}
