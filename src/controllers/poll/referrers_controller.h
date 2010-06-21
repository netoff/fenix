#pragma once

#include "fenix.h"

using namespace fenix::web::toolkit;

namespace PagesController
{
	static string get_referrers_feed(const string& site_id, const long timestamp1,
			const long timestamp2, const int type)
	{
		string data = "";
		ostringstream query;
		query << "return referrers.get_feed({id = [[" << site_id << "]], ts1 = "
			<< timestamp1 << ", ts2 = " << timestamp2 << ", type = " << type << "})";
		
		ScopedMiddleware mid;
		
		data = mid.query(query.str());
		
		mid.done();
		
		return data;
	}
	
	namespace Poll
	{
		FENIX_CONTROLLER(referrers)
		{
			string site_id;
			long timestamp1, timestamp2;
			int type;
			
			if( request.isRead() &&
					get_param(request["id"], site_id) &&
					get_param(request["t"], type) && 
					get_timestamp(request["sd"], timestamp1, tm_type::DAY) &&
					get_timestamp(request["ed"], timestamp2, tm_type::DAY))
			{
				if(
					this->_authenticate.authorize_user(site_id) &&
					(type == -1 || type == 2 || type == 3 || type == 4) )
				{
					long timestamp_d = datetime::timestamp_d(request._timestamp);
					long diff = timestamp2 - timestamp1;
					
					if(timestamp1 < timestamp2 && diff < 240 && timestamp2 <= timestamp_d + 1)
					{
						ostringstream response;
						
						response << "page.updateFromFeed(" 
							<< get_referrers_feed(site_id, timestamp1, timestamp2, type) << ");\n";
								
						return render_text(response, "text/javascript");
					}						
				}				
			}
			
			return render_<BadRequestResponse>();
		}
	}
}
