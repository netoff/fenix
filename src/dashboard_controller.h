#pragma once

#include "fenix.h"
#include "views.h"

using namespace fenix::web::toolkit;

using namespace pages;

namespace DashboardController
{
	FENIX_CONTROLLER(index)
	{
		return new Dashboard::Page(Dashboard::_request = request);
	}

}
