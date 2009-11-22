#pragma once
#include "fenix.h"

using namespace fenix::web::toolkit;

namespace pages
{
FENIX_PAGE_LAYOUT(AppLayout)
{
	this->insert("<html>\r\n\t<head>\r\n\t\t\r\n\t\t<title>Hello World</title>\r\n\t</head>\r\n\t<body>\r\n\t\t<div class=\"container\" id=\"container\">\r\n\t\t<div class=\"span-24\">\r\n\t\t", false);
	
	this->applay_layout();
	
	this->insert("\r\n\t\t</div>\r\n\t\t</div>\r\n\t</body>\r\n</html>\r\n", false);
}

//--------------------------------------------
//Begin File: Index.html
namespace Index
{
BOOST_PARAMETER_NAME(request)

FENIX_LAYOUTED_WEB_PAGE(Page, AppLayout, (required (request, *) ))
{
	this->insert("<h1>Hello World</h1>\n<p>Hello World Application</p>\n", false);
}
}
//End File

}
