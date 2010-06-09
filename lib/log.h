#pragma once

#include "application.h"

#include <stdio.h>

#include <sstream>

//#include <boost/thread/thread.hpp>
//#include <boost/thread/tss.hpp>

using namespace std;
using namespace boost;


namespace fenix
{
	namespace web
	{
		namespace toolkit
		{		
			namespace log
			{			
				class log
				{
				public:
					log(){}
					
					ostringstream& get()
					{
						return this->_out;
					}
					
					~log()
					{
						//only log in debug mode
						#ifdef DEBUG
						app.log(this->_out.str());
						#endif
					}
					
				private:
					ostringstream _out;
				};
				
				#define getLOG log().get()
			}
		}
	}
}
