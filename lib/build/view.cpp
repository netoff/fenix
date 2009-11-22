#include "view.h"

#include "boost/format.hpp"
#include "boost/algorithm/string.hpp"


using namespace fenix::web::toolkit::view;
using namespace boost;

void DHTMLResponse::insert(std::string a, bool new_line)
{
	if(new_line)
		_response.append('\n' + a);
	else
		_response.append(a);
}
std::string DHTMLResponse::escape(std::string s)
{	
	replace_all(s, "&", "&amp;");
	replace_all(s, "<", "&lt;");
	replace_all(s, ">", "&gt;");
	return s;
}

const char* FileResponse::mime_types[23][2] = {
	{".js", "application/javascript"},
	{".pdf", "application/pdf"},
	{".zip", "application/zip"},
	{".mp3", "audio/mpeg"},
	{".mwa", "audio/x-ms-wma"},
	{".wav", "audio/x-wav"},
	{".gif", "image/gif"},
	{".jpg", "image/jpeg"},
	{".jpeg", "image/jpeg"},
	{".png", "image/png"},
	{".svg", "image/svg+xml"},
	{".tiff", "image/tiff"},
	{".tif", "image/tiff"},
	{".css","text/css"},
	{".csv", "text/csv"},
	{".html", "text/html"}, 
	{".htm", "text/html"},
	{".txt", "text/plain"},
	{".xml", "text/xml"},
	{".mpg", "video/mpeg"},
	{".mp4", "video/mp4"},
	{".mov", "video/quicktime"},
	{".wmv", "video/x-ms-wmv"}};
