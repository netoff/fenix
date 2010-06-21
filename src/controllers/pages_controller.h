#pragma once

#include "fenix.h"
#include "views.h"

namespace PagesController
{
	FENIX_CONTROLLER(pages)
	{
		if(request.isRead())
		{
			return render_<Pages::Page>((_request = request, _active="pages-menu-item", 
				_time = datetime::format(request._timestamp, "%H:%M %d-%b-%Y GMT+0"), _user = this->_authenticate.get_user()));
		}
		
		return render_<BadRequestResponse>();
	}
}
