#pragma once

#include "fenix.h"
#include "views.h"

using namespace fenix::web::toolkit;

using namespace pages::Visitors;

namespace PagesController
{
	FENIX_CONTROLLER(visitors)
	{
		typedef Visitors::Page visitors_page;
		
		return new visitors_page(_request = request);
	}
	
	namespace Poll
	{
		FENIX_CONTROLLER(visitors)
		{
			string response = "";
			
			return new InlineResponse(response, "text/plain");
		}
	}
}
