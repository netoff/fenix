#pragma once

#include <fenix.h>

#include "log/view.h"
#include "log/click.h"
#include "log/event.h"

using namespace fenix::web::toolkit;

namespace LogController
{
	
	
	FENIX_CONTROLLER(lg)
	{
		log::log() << "New log request";
		
		string site_id;
		string event_type;
		
		if(
			get_param(params["_id"], site_id) && 
			get_param(params["_e"], event_type) )
		{
			if(event_type == "hit")
			{
				hash notes = request.getSysParams("notes");
				
				
				string url, title, referrer, country_code, search_query;
				long last_view; //seconds ago
				
				get_param(params["_u"], url);
				get_param(params["_t"], title);
				get_param(params["_r"], referrer);
				get_param(params["_tm"], last_view);
				get_param(params["_c"], country_code);
				
				log_page_view(site_id, url, title, referrer, country_code, search_query, last_view, request._timestamp);
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
		else
		{			
			return render_<BadRequestResponse>();
		}
	}
}
