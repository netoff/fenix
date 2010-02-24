#pragma once

#include "fenix.h"
#include "views.h"
#include "aaa.h"

using namespace fenix::web::toolkit;

using namespace pages::Visitors;

namespace PagesController
{
	FENIX_CONTROLLER(visitors)
	{
		return render_<Visitors::Page>((_request=request, _active="visits-menu-item", 
			_time = datetime::format(request._timestamp, "%H:%M %d-%b-%Y GMT"), _user = this->_authenticate.get_user()));
	}
	
	namespace Poll
	{
		FENIX_CONTROLLER(visitors)
		{
			string response = "";
			
			return render_text("", "text/plain");
		}
	}
}
