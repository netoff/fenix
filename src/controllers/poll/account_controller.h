#pragma once

#include "fenix.h"

using namespace fenix::web::toolkit;


namespace PagesController
{
	namespace Poll
	{
		namespace Account
		{
			FENIX_CONTROLLER(time)
			{
				if(request.isRead())
				{
					ostringstream response;
					
					string time = datetime::format(request._timestamp, "%H:%M %d-%b-%Y GMT+0");
					
					response << "$('#time').text('" << time << "');\n";
					
					return render_text(response, "text/javascript");
				}
				
				return render_<BadRequestResponse>();
			}
			
			FENIX_CONTROLLER(change_password)
			{
				if(request.isWrite())
				{				
					string old_password, new_password, repeat_password;
					
					if(get_param(request["old"], old_password) &&
						get_param(request["password"], new_password) &&
						get_param(request["repeat_password"], repeat_password))
					{
						ostringstream response;
						
						return render_text(response, "text/javascript");
					}
				}
				
				return render_<BadRequestResponse>();
			}
			
			FENIX_CONTROLLER(add_website)
			{
			}			
			
		}
	}
}
