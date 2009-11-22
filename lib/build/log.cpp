#include "log.h"

using namespace fenix::web::toolkit::server;


std::ostringstream& Log::get()
{
   //os << "- " << "Now Time!";
   //os << " " << level << ": ";
   //os << std::string(level > logDEBUG ? 0 : level - logDEBUG, '\t');
   return os;
}

Log::~Log()
{
	 os << std::endl;
	 fprintf(_log_file, "%s", os.str().c_str());
	 fflush(stderr);
}