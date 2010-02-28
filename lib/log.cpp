#include "log.h"

namespace fenix
{
	namespace web
	{
		namespace toolkit
		{
			namespace log
			{
				ostringstream _null_output;
				
				#ifdef DEBUG
				thread_specific_ptr<ostringstream> _log_output;
				#endif				
			}
		}
	}
}
