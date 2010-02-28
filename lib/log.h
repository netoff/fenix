#pragma once

#include <sstream>

#include <boost/thread/thread.hpp>
#include <boost/thread/tss.hpp>

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
				//is this safe?
				extern ostringstream _null_output;
				
				#ifdef DEBUG
				extern thread_specific_ptr<ostringstream> _log_output;
				#endif
				
				inline ostringstream& log()
				{
					#ifdef DEBUG
					if(!_log_output.get())
					{
						_log_output.reset(new ostringstream(""));
					}
					
					if(_log_output.get())
					{
						return *_log_output;
					}
					#endif
					
					//this output goes nowhere. in case _log_output is not defined
					//return this, because you can not return reference to stack object
					//_null_ouput is not thread safe, but it does not matter
					//it would do nothing anyway, just make code more "stable??"
					return _null_output;
				}
				
				inline string log_output()
				{
					#ifdef DEBUG
					if(_log_output.get())
					{
						return (*_log_output).str();
					}
					#endif
					
					return "";
				}
			}
		}
	}
}
