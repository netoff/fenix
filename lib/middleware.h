#pragma once

#include "common.h"
#include "log.h"

#include <boost/asio.hpp>
#include <boost/thread.hpp>

#include <stack>

using namespace boost::asio;

namespace fenix
{
	namespace web
	{
		namespace toolkit
		{
			//static const int middleware_port=13131;
			static const string local_endpoint = "/tmp/fenix";
			
			static const unsigned int max_pool_size = 256;
			
			class Middleware: private noncopyable
			{
			public:
				Middleware()
				:_ios(),_socket(_ios),_bad(false)
				{
					asio::local::stream_protocol::endpoint ep(local_endpoint);
					this->_socket.connect(ep);
				}
				
				string query(const string& code) const
				{
					return this->_query(code);
				}
				
				void insert(const string& code) const
				{
					this->_insert(code);
				}
				
				bool is_bad() const
				{
					return !this->_socket.is_open() || this->_bad;
				}
			private:
				io_service _ios;
				asio::local::stream_protocol::socket _socket;
				bool _bad;
				
				string _connect(const string& header, const string& msg);
				
				string _query(const string& code)
				{
					return this->_connect("query", code);
				}
				void _insert(const string& code)
				{
					this->_connect("insert", code);
				}
				
			};
			
			class MiddlewarePool:private noncopyable
			{
			public:
				Middleware* get()
				{
					Middleware* ret = 0;
					
					{
						mutex::scoped_lock L(this->_mutex);						
						
						if(this->_pool.empty())
						{
							try
							{
								auto_ptr<Middleware> autoptr(new Middleware());
								
								if(autoptr.get())
								{
									ret = autoptr.get();
									autoptr.release();
								}
							}
							catch(...)
							{//maybe it is not possible to get connection}
							}
						}
						else
						{
							ret = this->_pool.top();
							this->_pool.pop();
						}
						
					}
				
					return ret;
				}
				
				void release(Middleware* m)
				{
					if(m)
					{
						if(m->is_bad())
						{
							delete m;
							return;
						}
						
						{
							mutex::scoped_lock L(this->_mutex);
							
							if(this->_pool.size() > max_pool_size)
							{
								log::getLOG << "Releasing connection in pool";
								delete m;
								return;
							}
							
							this->_pool.push(m);
						}
					}
				}
			private:
				mutex _mutex;
				stack<Middleware*> _pool;				
			};
			
			extern MiddlewarePool mid_pool;
			
			class ScopedMiddleware: private noncopyable
			{
			public:
				ScopedMiddleware()
				:_mid(mid_pool.get())
				{
				}
				
				~ScopedMiddleware()
				{
					if(this->_mid)
					{
						delete this->_mid;
					}
				}
				
				void done()
				{
					if(this->_mid)
					{
						mid_pool.release(this->_mid);
					}
					
					this->_mid = 0;
				}
				
				string query(const string& code) const 
				{
					this->_assert();
					
					if(this->_mid && !this->_mid->is_bad())
					{
						return this->_mid->query(code);
					}
					
					return "";
				}
				void insert(const string& code) const 
				{
					this->_assert();
					
					if(this->_mid && !this->_mid->is_bad())
					{
						this->_mid->insert(code);
					}
				}
				
			private:
				Middleware* _mid;
				
				//thorw away bad connections
				void _assert()
				{
					if(!this->_mid)
					{
						this->_mid = mid_pool.get();
					}
					else if(this->_mid->is_bad())
					{
						log::getLOG << "Releaseing connection in assert!";
						delete this->_mid;
						
						this->_mid = mid_pool.get();
					}
				}
			};
			
		}
	}
}
