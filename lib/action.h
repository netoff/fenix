#pragma once

#include <memory>
#include "common.h"
#include "datetime.h"
#include "view.h"
#include "params.h"
#include "crypto.h"

#define BOOST_PARAMETER_MAX_ARITY 15

#include "boost/parameter.hpp"
#include "boost/date_time/posix_time/posix_time.hpp"
#include "boost/tokenizer.hpp"
#include "boost/typeof/typeof.hpp"

using namespace boost;
using namespace boost::parameter;

namespace fenix
{
	namespace web
	{
		namespace toolkit
		{
			namespace action
			{
				bool _get_val(const string& raw, string& val);
				
				string _get_val(const string& val);
				
				struct cookie_param
				{					
					cookie_param(){}
					
					cookie_param(const string& v, const int e)
					:val(v),path("/"),domain(""),expire(e) {	}
					
					cookie_param(const string& v, const string& p, const string& d, const int e)
					:val(v),path(p),domain(d),expire(e) { }
					
					string val, path, domain;
					int expire;
				};
				class Request: private noncopyable
				{
				public:
					enum RequestType
					{
						READ,
						WRITE,
						UNKNOWN
					};
					enum RequestVersion
					{
						v1_0
					};

					typedef RequestType Type;
					typedef RequestVersion Version;

					string getType()const
					{
						switch(_type)
						{
						case READ:
							return "Read";
						case WRITE:
							return "Write";
						default:
							return "Unknown";
						}
					}

					Request() { _timestamp = microsec_clock::universal_time(); }

					bool isRead()const
					{
						if(_type == READ)
						{
							return true;
						}
						
						return false;
					}
					bool isWrite()const 
					{
						if(_type == WRITE)
						{
							return true;
						}
						
						return false;
					}
					
					typedef cookie_param pparam;
					
					string getParam(const string& key) const
					{
						map<string, string>::const_iterator iter = _params.find(key);
						
						if(iter != _params.end())
						{
							return iter->second;
						}
						
						return "";
					}
					
					string getParam(const string& ns, const string& key) const
					{
						map<string, map<string, string> >::const_iterator iter = _sparams.find(ns);
						
						if(iter != _sparams.end())
						{
							map<string, string>::const_iterator iter1 = iter->second.find(key);
							
							if(iter1 != iter->second.end())
							{
								return iter1->second;
							}
						}
						
						return "";
					}
					
					void setParam(const string& key, const string& val)
					{
						_params[key] = val;
					}
					
					void setParam(const string& ns, const string& key, const string& val)
					{
						_sparams[ns][key] = val;
					}
					
					const string operator()(const string& key) const
					{
						return getParam(key);
					}
					
					const string operator()(const string& ns, const string& key) const
					{
						return getParam(ns, key);
					}
					
					const string operator[](const string& key) const
					{
						return getParam(key);
					}					
					
					const string getSession(const string& key) const
					{
						map<string, pparam>::const_iterator iter = _pparams.find(key);
						
						string ret = "";
						
						if(iter != _pparams.end())
						{
							if(_get_val(iter->second.val, ret))
							{
								return ret;
							}
						}
						
						return ret;
					}
					
					void clearSession(const string& domain)
					{
						typedef pair<string, cookie_param> cookie_pair;
						
						foreach(cookie_pair cookie, _pparams)
						{
							_pparams[cookie.first] = cookie_param("", "/", domain, 0);
						}
					}
					
					void setSession(const string& key, const string& val, int expire = 0)
					{						
						_pparams[key] = cookie_param(_get_val(val), expire);
					}
					
					void setSession(const string& key, const string& val, 
						const string& domain, int expire = 0)
					{
						_pparams[key] = cookie_param(_get_val(val), "/", domain, expire);
					}
					
					/*
					void setSessionRaw(const string& key, const string& val)
					{
						string v;
						
						if(_get_val(val, v))
						{						
							this->_pparams[key] = cookie_param(v, -1);
						}
					}*/

					string _server_name;
					string _home_path;

					uint64_t _unix_timestamp;
					string _md5;

					//This should be core API
					//TODO: Document and Explain each member
					//Please pay attention that this does not have anything to do
					//with HTTP request. This request is not required to be run in HTTP
					//environment at all. Eg. requests that are coming from job scheduler

					//Query: Query string that uniquely(?!?!) identifies this request
					vector<string> _query;
					//Params: Request params. In case of HTTP env this is taken from GET or POST params,
					//or it could be params on the command line, params from the config file, etc...
					map<string, string> _params;
					
					//PParams: Persistent params. With HTTP protocol these are cookie values...
					//TODO: Is persistent storage changable by user?? To enable customization of session storage
					//provider. Or it should be based on every environment type, to provide presistent storage...
					map<string, pparam> _pparams;
					
					//SParams: System params. This could be HTTP meta params, like referer, user-agent...Or something
					//else that depends on environment it executes in. Nither of these params class is guarantied to 
					//be there.
					map<string, map<string, string> > _sparams;

					ptime _timestamp;
					
					Type _type;
					
					//host parts are inserted in revers order 1:com, 2:kliknik, 3:www etc.
					deque<string> _host;
					string _ip;
					
					/*
					void add_note(shared_ptr<Note> n)
					{
						_notes.push_back(n);
					}*/
					
					~Request()
					{
					}
					
				private:					
					
					std::ostringstream _log;
					
					string _app_context;
					
					/*
					vector<shared_ptr<Note> > _notes;*/
				};
				
				class Base
				{
				public:
					virtual ~Base() {}
					
					virtual shared_ptr<view::Response> handle(const Request& request) = 0;
				};
				
				template <class Authenticate>
				class Action: public Base
				{
				public:
					Action() {}
					
					virtual shared_ptr<view::Response> handle(const Request& request)
					{		
						_authenticate = Authenticate(request);
						
						string redirect = "";
		  				
						if(_authenticate(request, redirect))				
						{
							return _handle(request);							
						}
						
						if(!redirect.empty())						
						{								
							return view::render_redirect(redirect);			
						}							
						  
						return view::render_<view::ForbidenResponse>();				
					}

					virtual ~Action(){}
				protected:
					 Authenticate _authenticate;
				private:
					Action(const Action& rhs) {}
					Action& operator=(const Action& rhs) {}
					
					virtual shared_ptr<view::Response> _handle(const Request& request) = 0;	
				};
				
				template<>
				inline shared_ptr<view::Response> Action<empty_class>::handle(const Request& request)
				{
					return _handle(request);
				}
			}
		}
	}
}

#define VIEW_CLASS(x) view_##x

//TODO: Refactor DHTMLResponseclass
#define FENIX_WEB_PAGE(page_name, params)					\
class VIEW_CLASS(page_name):public view::DHTMLResponse, private noncopyable\
{										\
	public:									\
	template <class ArgumentPack>						\
	VIEW_CLASS(page_name)(ArgumentPack const& args);			\
};										\
typedef VIEW_CLASS(page_name) page;						\
class page_name:public page 							\
{										\
	public:									\
	template <class ArgumentPack>						\
	page_name(const ArgumentPack& args):page(args){}			\
};										\
template <class ArgumentPack> VIEW_CLASS(page_name)::VIEW_CLASS(page_name)(ArgumentPack const& args)


#define FENIX_LAYOUTED_WEB_PAGE(page_name, layout)			\
class VIEW_CLASS(page_name):public view::DHTMLResponse, private noncopyable\
{																		\
	public: 													\
	VIEW_CLASS(page_name)(){}					\
	virtual ~VIEW_CLASS(page_name)(){}\
	template <class ArgumentPack>						\
	VIEW_CLASS(page_name)(ArgumentPack const& args);			\
};										\
typedef layout<VIEW_CLASS(page_name)> layouted_page;				\
class page_name:public layouted_page						\
{										\
		public:								\
		template <class ArgumentPack>					\
		page_name(const ArgumentPack& args):layouted_page(args){}	\
		virtual ~page_name(){}\
};										\
template <class ArgumentPack> VIEW_CLASS(page_name)::VIEW_CLASS(page_name)(ArgumentPack const& args)


#define FENIX_CONTROLLER(controller)						\
template <class Authenticate = empty_class>					\
class controller: public action::Action<Authenticate>, private noncopyable\
{													\
public:										\
	controller() {}					\
private:									\
	virtual shared_ptr<view::Response>					\
	  	_handle(const action::Request& request);			\
};										\
template <class Authenticate>							\
shared_ptr<view::Response> controller<Authenticate>::_handle(const action::Request& request)





