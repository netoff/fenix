/*#include "log.h"
#include "application.h"

using namespace fenix::web::toolkit;

extern "C" __attribute__((visibility("default"))) void appinit(FILE* error_log)
{
	log::_init_log(error_log);
}*/
#include "application.h"

namespace fenix
{
	namespace web
	{
		namespace toolkit
		{
			Application app;
		}
	}
}