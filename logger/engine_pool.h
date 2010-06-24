#pragma once

#include <stack>
#include <boost/thread.hpp>
#include "common.h"

#include "engine.h"
#include "log.h"

static const unsigned int max_pool_size = 512;

namespace fenix
{
	namespace misc
	{
		namespace server
		{
			class EnginePool: public noncopyable
			{
			public:
				EnginePool()
				{
					this->_stored = "";
				}
				~EnginePool()
				{
					while(!this->_pool.empty())
					{
						engine* e = this->_pool.top();
						this->_pool.pop();
						
						delete e;
					}
				}
				
				void set_stored(const string& s)
				{
					this->_stored = s;
				}
				
				engine* get()
				{
					engine* ret = 0;
					
					{//mutex scope
						mutex::scoped_lock L(this->_mutex);
					
						if(this->_pool.empty())
						{
							auto_ptr<engine> apr(new engine(this->_stored));
							
							ret = apr.get();
							apr.release();
							
							Log().get() << "Created new LUA engine.";
						}
						else
						{
							ret = this->_pool.top();
							this->_pool.pop();
						}
					}
					
					return ret;
				}
				
				void release(engine* e)
				{					
					if(e)
					{
						if(!e->is_bad() && this->_pool.size() < max_pool_size)
						{
							mutex::scoped_lock L(this->_mutex);
							
							this->_pool.push(e);
							
							return;
						}
						else
						{						
							delete e;
						}
					}
				}
			private:
				stack<engine*> _pool;
				mutex _mutex;
				
				string _stored;
			};
			
			extern EnginePool engine_pool;
			
			class ScopedEngine:public noncopyable
			{
			public:
				ScopedEngine()
				:_engine(engine_pool.get())
				{
				}
				~ScopedEngine()
				{
					if(this->_engine)
					{
						delete this->_engine;
					}
				}
				
				void done()
				{
					if(this->_engine)
					{
						engine_pool.release(this->_engine);
						this->_engine = 0;
					}
				}
				
				void renew()
				{
					if(this->_engine)
					{
						delete this->_engine;
						this->_engine = engine_pool.get();
					}
				}
				
				engine* operator->()
				{
					return this->_engine;
				}
				
			private:
				engine* _engine;				
			};
		}
	}
}
