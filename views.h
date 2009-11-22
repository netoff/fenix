#pragma once
#include "common.h"
#include "models.h"
#include "view.h"
#include "controllers.h"

namespace pages
{
//--------------------------------------------
//Begin File: Index.html
namespace Index
{
BOOST_PARAMETER_NAME(request)

FENIX_LAYOUTED_WEB_PAGE(Page, AppLayout, (required (request, *) ))
{
	insert("<h1>Hello World</h1>\n<p>Hello World Application</p>\n", false);
}
}
//End File

}
