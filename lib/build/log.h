#pragma once

#include "stdio.h"
#include <sstream>

using namespace std;

namespace fenix
{
	namespace web
	{
		namespace toolkit
		{
			namespace server
			{
				class Log
				{
				public:
					enum LogLevel 
					{
						logERROR, 
						logWARNING, 
						logINFO, 
						logDEBUG
					};

					typedef LogLevel Level;

					Log(FILE* log_file, Level level)
						:_log_file(log_file),_level(level){}
					virtual ~Log();

					std::ostringstream& get();
				protected:
					std::ostringstream os;

				private:
					Log(const Log&);
					Log& operator =(const Log&);

				private:
					FILE* _log_file;
					Level _level;
				};
			}
		}
	}
}
