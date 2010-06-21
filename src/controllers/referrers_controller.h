#pragma once

#include "fenix.h"
#include "views.h"

namespace PagesController
{
	FENIX_CONTROLLER(referrers)
	{
		if(request.isRead())
		{
			return render_<Referrers::Page>((_request = request, _active="referrers-menu-item", 
				_time = datetime::format(request._timestamp, "%H:%M %d-%b-%Y GMT+0"), _user = this->_authenticate.get_user()));
		}
		
		return render_<BadRequestResponse>();
	}
}
