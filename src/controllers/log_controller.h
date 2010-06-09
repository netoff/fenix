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
			request.isRead() &&
			get_param(request["_id"], site_id) && 
			get_param(request["_e"], event_type))
		{			
			tables::Site::obj site = Site::get(get_database(request, "fenix"));
			
			//ID is 24 chars long
			if(site_id.size() == 24 && site->exists(Query().add_cond("_id", site_id)))
			{
				
				if(event_type == "hit")
				{
					long last_view; //seconds ago
					
					get_param(request["_tm"], last_view);
					
					{
						LogRequest log_request(site_id, event_type);
						log_request.load(request);
						
						ScopedMiddleware mid;
						
						log_page_view(log_request, last_view, request._timestamp, mid);
						
						mid.done();
					}
				}
				/*
				if(event_type == "cl")
				{
					log_click();
				}
				if(event_type == "ev")
				{
					log_event();
				}*/
								
				string response = "view log";
				
				//return render_text(response, "text/plain");				
				return render_<TrackingPixel>();
			}
		}
		
		return render_<BadRequestResponse>();		
	}
}
