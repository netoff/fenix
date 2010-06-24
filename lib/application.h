#pragma once

#include "common.h"
#include "geo.h"
//#include "action.h"
//#include "model.h"
//#include "view.h"
//#include "log.h"

#include <stdio.h>

static const string log_file = "/var/log/app.log";
static const string geo_ip_file ="/opt/geo/GeoIPCity.dat";

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
				:_error_log(fopen(log_file.c_str(), "a")),
				_geo_location(geo_ip_file)
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
				
				template <class Obj>
				void geo_lookup(Obj& o, const string& ip)
				{
					this->_geo_location(o, ip);
				}
			private:
				FILE* _error_log;		
				geo::Location _geo_location;
			};
			
			extern Application app;
		}
	}
}

#define FENIX_APPLICATION(app)								\
	extern "C" __attribute__((visibility("default"))) 				\
			shared_ptr<view::Response> handle(const action::Request& request)
