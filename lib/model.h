#pragma once

#include "common.h"
#include "params.h"
#include "model_obj.h"

using namespace fenix::web::toolkit::action;

namespace fenix
{
	namespace web
	{
		namespace  toolkit
		{
			namespace model
			{
				class DatabaseImpl;

				class Database
				{
					
				public:					
					typedef shared_ptr<Database> obj;
					
					Database(const string& host, const string& db, int port);
					
					~Database(){}					

					DatabaseObj find(const string& query, const string& table);
					
					DatabaseColl find_all(const string& query, const string& table);
					
					string save(const string& id, const string& bson, const string& table);
					
					string eval(const string& query);
					
					bool exists(const string& query, const string& table);	
					
					//void notify();
				private:
					Database(const Database& rhs){}
					Database& operator=(const Database& rhs){}
					
					string get_ns(const string& table)
					{
						return this->_database + "." + table;
					}
					
					string _host;
					string _database;
					int _port; 
					
					auto_ptr<DatabaseImpl> _db_impl;
				};
			}
		}
	}
}
