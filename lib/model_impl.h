#pragma once

#include <string>
#include <memory>
#include "boost/shared_ptr.hpp"

#include "model_obj.h"

namespace fenix
{
	namespace web
	{
		namespace toolkit
		{
			namespace model
			{
				class MongoImpl;
				
				class DatabaseImpl
				{
				public:
					DatabaseImpl(const std::string& host, int port);
					
					DatabaseObj find(const std::string& query, const std::string& ns);
					
					DatabaseColl find_all(const std::string& query, const std::string& ns);
					
					std::string save(const std::string& id, const std::string& bson, const std::string& ns);
					
					std::string eval(const std::string& query, const std::string& db);
					
					bool exists(const std::string& query, const std::string& ns);
					
					//void notify();
				private:
					DatabaseImpl(const DatabaseImpl& rhs){}
					DatabaseImpl& operator=(const DatabaseImpl& rhs){}
					
					std::string _host;
					int _port;
					
					std::auto_ptr<MongoImpl>  _conn;
				};
			}
		}
	}
}
