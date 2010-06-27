#pragma once

#include "fenix.h"

using namespace fenix::web::toolkit;

namespace PagesController
{	
	static string get_visitors_feed(const ScopedMiddleware& mid, 
		const string& site_id, const long ts1, const long ts2, const int type)
	{
		ostringstream query;
		query << "return visitors.get_feed({id = [[" << site_id << "]], ts1 = "
			<< ts1 << ", ts2 = " << ts2 << ", type = " << type << "})";
			
		string data = mid.query(query.str());
		
		return data;
	}
	
	namespace Poll
	{
		FENIX_CONTROLLER(visitors)
		{
			string site_id;
			
			long timestamp1;
			long timestamp2;
			int type;
						
			if(	request.isRead() &&
					get_param(request["id"], site_id) &&
					get_param(request["t"], type) &&
					get_timestamp(request["sd"], timestamp1, tm_type::DAY) &&
					get_timestamp(request["ed"], timestamp2, tm_type::DAY))
			{
				if(this->_authenticate.authorize_user(site_id))
				{
					long diff = timestamp2 - timestamp1;
					
					long timestamp_d = datetime::timestamp_d(request._timestamp);
					
					//Within allowed range. Should be less than maximal span
					if(timestamp1 < timestamp2 && diff < 240 && timestamp2 <= timestamp_d + 1)
					{
						if( type == -1 || type == 2 ||
								type == 3 || type == 4)
						{
							ostringstream response;
							
							ScopedMiddleware mid;
							
							response << "page.updateFromFeed(" 
								<< get_visitors_feed(mid, site_id, timestamp1, timestamp2, type) << ")";
							
							mid.done();
							
							return render_text(response, "text/javascript");
						}				
					}
				}
			}
			
			return render_<BadRequestResponse>();
		}
	}
}
