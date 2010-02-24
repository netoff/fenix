#pragma once

#include "fenix.h"
#include "views.h"

namespace PagesController
{
	FENIX_CONTROLLER(referrers)
	{
		return render_<Referrers::Page>((_request = request, _active="dashboard-menu-item", 
			_time = datetime::format(request._timestamp, "%H:%M %d-%b-%Y GMT"), _user = this->_authenticate.get_user()));
	}
}
