#pragma once

#include "common.h"
//#include "action.h"
//#include "model.h"
//#include "view.h"
//#include "log.h"

#include <stdio.h>

namespace fenix
{
	namespace web
	{
		namespace toolkit
		{
			class Application
			{
			public:
				Application()
				:_error_log(fopen("/opt/log/app.log", "a"))
				{
				}
				
				~Application()
				{
					if(this->_error_log)
					{
						fclose(this->_error_log);
					}
				}
				
				void log(const string& msg)
				{
					if(this->_error_log)
					{
						fprintf(this->_error_log, "(log): %s\n", msg.c_str());
						fflush(this->_error_log);
					}
				}
			private:
				FILE* _error_log;				
			};
			
			extern Application app;
		}
	}
}

#define FENIX_APPLICATION(app)								\
	extern "C" __attribute__((visibility("default"))) 				\
			shared_ptr<view::Response> handle(const action::Request& request)
