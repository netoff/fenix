#pragma once

#include "lua.h"

#include "common.h"
#include "log.h"
#include "send_mail.h"

namespace fenix
{
	namespace misc
	{
		namespace server
		{
			extern "C" {int luaopen_mongo(lua_State *L);}
			
			class engine: public noncopyable
			{
			public:
				engine(const string& stored)
				:_L(luaL_newstate()),_bad(false)
				{
					luaL_openlibs(_L);
					luaopen_mongo(_L);
					luaL_dostring(_L, stored.c_str());
					
					lua_register(_L, "_log", _lua_log);
					lua_register(_L, "_send_mail", _lua_send_mail);
				}
				~engine()
				{
					if(_L)
					{
					 lua_close(_L);
					}
				}
				
				bool proc(const string& code, string& result, bool ret = true)
				{
					result = "";
					
					if(luaL_loadstring(_L, code.c_str()))
					{
						Log().get() << "Error trying to load code: " << code << endl
							<< "(error): " << lua_tostring(_L, -1);
							
						return false;
					}
					
					if(lua_pcall(_L, 0, LUA_MULTRET, 0))
					{
						Log().get() << "Error trying to execute code: "<< code << endl
							<< "(error): " << lua_tostring(_L, -1);
							
						this->_bad = true;
							
						return false;
					}
					else
					{
						if(!ret)
						{
							return true;
						}
						else
						{
							if(lua_isstring(_L, -1))
							{
								result = lua_tostring(_L, -1);
								lua_pop(_L, 1);
								
								return true;
							}
						}
					}		
					
					return false;
				}
				
				bool proc(const string& code)
				{
					string null = "";
					return this->proc(code, null, false);
				}
				
				bool is_bad()
				{
					return this->_bad;
				}
			private:
				lua_State *_L;
				bool _bad;
			};
		}
	}
}
