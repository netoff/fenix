#pragma once

#include "fenix.h"
#include "views.h"

using namespace fenix::web::toolkit;

using namespace pages;

namespace HelloController
{
	FENIX_CONTROLLER(index)
	{
		return new Index::Page(Index::_request = request);
	}

}
