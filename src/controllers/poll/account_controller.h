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
		}
	}
}
