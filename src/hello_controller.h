#pragma once

#include "fenix.h"
#include "views.h"

using namespace fenix::web::toolkit;

namespace HelloController
{
	FENIX_CONTROLLER(index)
	{
		using namespace pages::Index;
		return new Page(_request = request);
	}

}
