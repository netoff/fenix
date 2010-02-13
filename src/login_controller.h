#pragma once

#include <fenix.h>
#include "views.h"

using namespace fenix::web::toolkit;
using namespace pages;

namespace LoginController
{
	FENIX_CONTROLLER(index)
	{
		return render_<Login::Page>((Login::_request = request));
	}
	
	/*FENIX_CONTROLLER(login)
	{
	}
	
	FENIX_CONTROLLER(logout)
	{
	}*/
}
