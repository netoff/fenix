#include "model_impl.h" 
#include "client/dbclient.h"
#include "client/connpool.h"

namespace fenix
{
	namespace web
	{
		namespace toolkit
		{
			namespace model
			{
				class MongoImpl
				{
				public:
					MongoImpl(const std::string& host, int port)
					:_host(host)
					{
						//this->_conn.connect(host);
					}
					~MongoImpl()
					{
						//this->_conn.done();
					}
					
					DatabaseObj find(const std::string& query, const std::string& ns)
					{
						mongo::ScopedDbConnection conn(this->_host);
						
						mongo::BSONObj res = conn->findOne(ns, mongo::Query(mongo::fromjson(query)));
						
						DatabaseObj ret = this->bson_to_obj(res);
						
						conn.done();
						
						return ret;
					}
					
					DatabaseColl find_all(const std::string& query, const std::string& ns)
					{
						DatabaseColl ret;
						
						mongo::ScopedDbConnection conn(this->_host);
						std::auto_ptr<mongo::DBClientCursor> c = conn->query(ns, mongo::Query(mongo::fromjson(query)));
						
						while(c->more())
						{
							mongo::BSONObj bson = c->next();
							DatabaseObj obj = this->bson_to_obj(bson);
							
							ret.push_back(obj);
						}
						
						conn.done();
						
						return ret;
					}
					
					std::string save(const std::string& id, const std::string& bson, const std::string& ns)
					{
						const char* id_name = "_id";
						
						std::string oid;
						
						mongo::OID _oid;
						mongo::BSONObjBuilder b;
						
						mongo::ScopedDbConnection conn(this->_host);						
						
						if(!id.empty())
						{							
							_oid.init(id);
							oid = _oid.str();
							
							b.appendOID(id_name, &_oid);							
							conn->update(ns, mongo::Query(b.obj()), mongo::fromjson(bson));
						}
						else
						{
							_oid.init();
							oid = _oid.str();
							
							b.appendOID(id_name, &_oid);
							b.appendElements(mongo::fromjson(bson));
							
							conn->insert(ns, b.obj());
						}
						
						conn.done();
						
						return oid;
					}
					
					std::string eval(const std::string& db, const std::string& query)
					{						
						std::string ret;

						mongo::BSONObj info;
						mongo::BSONElement res;
						
						mongo::ScopedDbConnection conn(this->_host);
						conn->eval(db, query, info, res);
						
						ret = res.str();
						
						conn.done();
						
						return ret;						
					}
					
					bool exists(const std::string& query, const std::string& ns)
					{
						mongo::ScopedDbConnection conn(this->_host);
						mongo::BSONObj res = conn->findOne(ns, mongo::Query(mongo::fromjson(query)));
						
						conn.done();
						
						return !res.isEmpty();
					}
					
					/*
					void notify()
					{
						this->_conn.done();
					}*/
				private:
					DatabaseObj bson_to_obj(mongo::BSONObj& o)
					{
						DatabaseObj ret(new _DatabaseObj());
												
						mongo::BSONObjIterator it(o);
						
						while (it.more())
						{
							mongo::BSONElement e = it.next();
							
							if(std::string(e.fieldName()) == "_id" )
							{
								if(e.type() == mongo::jstOID)
								{
									ret->_id = e.__oid().str();
								}
							}
							else
							{
								if(!e.eoo())
								{
									std::string key = e.fieldName();
									std::string val = e.str();
								
									ret->_cols[key] = val;
								}
							}
						}
						
						
						return ret;
					}					
					
					std::string _host;
					//mongo::DBClientConnection _conn;
					//mongo::ScopedDbConnection _conn;
				};
				
				DatabaseImpl::DatabaseImpl(const std::string& host, int port)
				:_host(host),_port(port),_conn(new MongoImpl(host, port))
				{
				}
				
				DatabaseObj DatabaseImpl::find(const std::string& query, const std::string& ns)
				{
					return this->_conn->find(query, ns);
				}
				DatabaseColl DatabaseImpl::find_all(const std::string& query, const std::string& ns)
				{
					return this->_conn->find_all(query, ns);
				}
				std::string DatabaseImpl::save(const std::string& id, const std::string& bson, const std::string& ns)
				{
					return this->_conn->save(id, bson, ns);
				}
				
				std::string DatabaseImpl::eval(const std::string& query, const std::string& db)
				{
					return this->_conn->eval(db, query);
				}
				
				bool DatabaseImpl::exists(const std::string& query, const std::string& ns)
				{
					return this->_conn->exists(query, ns);
				}
				
				/*
				
				void DatabaseImpl::notify()
				{
					return this->_conn->notify();
				}*/
			}
		}
	}
}
