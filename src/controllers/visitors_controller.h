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
		if(request.isRead())
		{
			return render_<Visitors::Page>((_request=request, _active="visitors-menu-item", 
				_time = datetime::format(request._timestamp, "%H:%M %d-%b-%Y GMT+0"), _user = this->_authenticate.get_user()));
		}
		
		return render_<BadRequestResponse>();
	}
}
