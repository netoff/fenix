#pragma once

#include <fenix.h>
#include "views.h"
#include "aaa.h"

#include "model/models.h"

using namespace fenix::web::toolkit;
using namespace pages;

namespace DashboardController
{
	FENIX_CONTROLLER(index)
	{	
		if(request.isRead())
		{
			return render_<Dashboard::Page>((_request = request, _active="dashboard-menu-item", 
				_time = datetime::format(request._timestamp, "%H:%M %d-%b-%Y GMT+0"), _user = this->_authenticate.get_user()));
		}
		
		return render_<BadRequestResponse>();
	}

}
