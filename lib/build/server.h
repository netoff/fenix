#pragma once

#include "stdio.h"
#include "log.h"

namespace fenix
{
	namespace web
	{
		namespace toolkit
		{
			namespace server
			{
				class Server
				{
				public:
					Server(FILE* log_file)
						:_log_file(log_file), _log(_log_file, Log::logDEBUG){}

					ostringstream& get_log()
					{
						return _log.get();
					}
				private:
					FILE* _log_file;
					Log _log;
				};
			}
		}
	}
}
