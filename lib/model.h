#pragma once

#include "common.h"
#include "params.h"

using namespace fenix::web::toolkit::action;

#include <tcrdb.h>

namespace fenix
{
	namespace web
	{
		namespace  toolkit
		{
			namespace model
			{

				class Database
				{
					//TODO: Handle databae error conditions
				public:
					Database(string host, int port)
						:_tyrant_server(tcrdbnew())
					{
						if(!tcrdbopen(_tyrant_server, host.c_str(), port))
						{
						    //ecode = tcrdbecode(rdb);
						    //fprintf(stderr, "open error: %s\n", tcrdberrmsg(ecode));
						}
					}
					~Database()
					{
						if(!tcrdbclose(_tyrant_server))
						{
						    //ecode = tcrdbecode(rdb);
						    //fprintf(stderr, "close error: %s\n", tcrdberrmsg(ecode));
						}

						tcrdbdel(_tyrant_server);
					}

					template<class T>
					T get(const string& key)
					{
						T ret;
						char * value = tcrdbget2(_tyrant_server, key.c_str());

						if(value)
						{
						    string val(value);
						    free(value);

						    get_param(val, ret);
						}
						else
						{
							//ecode = tcrdbecode(rdb);
						    //fprintf(stderr, "get error: %s\n", tcrdberrmsg(ecode));
						}
						return ret;
					}

					string get(const string& key)
					{
						char * value = tcrdbget2(_tyrant_server, key.c_str());

						if(value)
						{
							string val(value);
							free(value);

							return val;
						}
						else
						{
							//ecode = tcrdbecode(rdb);
							//fprintf(stderr, "get error: %s\n", tcrdberrmsg(ecode));
							return "";
						}
					}

					bool put(const string& key, const string& val)
					{
						return true;
					}
				private:
					TCRDB* _tyrant_server;
				};
			}
		}
	}
}
