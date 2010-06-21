#pragma once

#include "common.h"
#include "boost/thread.hpp"

#include "lua.h"

using namespace boost;

namespace fenix
{
	namespace misc
	{
		namespace server
		{
			extern mutex out_mutex;
			
			class Log: public noncopyable
			{
			public:
				Log()
				{
				}
				
				ostringstream& get()
				{
					return this->_out;
				}
				~Log()
				{
					//#ifdef DEBUG
					mutex::scoped_lock(out_mutex);
					cerr << this->_out.str() << endl;
					//#endif
				}
			private:
				
				ostringstream _out;
			};
			
			static int _lua_log(lua_State* L)
			{
				if(lua_isstring(L, 1))
				{
					string msg = string(lua_tostring(L, 1));
					lua_pop(L, 1);
					
					Log().get() << "(lua debug): " << msg; 
				}
				
				
				return 0;
			}
		}
	}
}
