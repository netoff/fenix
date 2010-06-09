#include "model.h"
#include "model_impl.h"

#include "log.h"

namespace fenix
{
	namespace web
	{
		namespace toolkit
		{
			namespace model
			{
				
				Database::Database(const string& host, const string& db, int port)
				:_host(host),_database(db),_port(port),_db_impl(new DatabaseImpl(host, port))
				{
					
				}
				
				DatabaseObj Database::find(const string& query, const string& table)
				{
					string ns = this->get_ns(table);
					
					return this->_db_impl->find(query, ns);
				}
				
				DatabaseColl Database::find_all(const string& query, const string& table)
				{
					string ns = this->get_ns(table);
					
					return this->_db_impl->find_all(query, ns);	
				}
				
				string Database::save(const string& id, const string& bson, const string& table)
				{
					string ns = this->get_ns(table);
					
					return this->_db_impl->save(id, bson, ns);
				}
				
				string Database::eval(const string& query)
				{
					string ret;
					
					ret = this->_db_impl->eval(query, this->_database);
					
					return ret;
				}
				
				bool Database::exists(const string& query, const string& table)
				{
					string ns = this->get_ns(table);
					
					return this->_db_impl->exists(query, ns);
				}
				
				/*
				void Database::notify()
				{
					this->_db_impl->notify();
				}*/
			}
		}
	}
}