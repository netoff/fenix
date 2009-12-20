#pragma once

#include <memory>
#include "common.h"
#include "datetime.h"
#include "server.h"
#include "view.h"
#include "params.h"

#define BOOST_PARAMETER_MAX_ARITY 15

#include "boost/parameter.hpp"
#include "boost/date_time/posix_time/posix_time.hpp"
#include "boost/tokenizer.hpp"

using namespace boost;
using namespace boost::parameter;

using namespace fenix::web::toolkit::server;

namespace fenix
{
	namespace web
	{
		namespace toolkit
		{
			namespace action
			{
				struct empty{};

				class Request
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

					Request()
						:_timestamp(microsec_clock::universal_time()){}

					bool isRead()const
					{
						if(_type == READ)
							return true;
						return false;
					}
					bool isWrite()const 
					{
						if(_type == WRITE)
							return true;
						return false;
					}


					//TODO: Should remove appContext??
					//I don't like that
					void setAppContext(string context){_app_context = context;}
					string appContext()const{return _app_context;}

					//TODO: Remove welcome page link too
					string welcomePageLink()const{return _app_context+"/Welcome";}
					string dispatcherPageLink()const{return _app_context+"/Dispatcher";}
 
					const map<string, string>& getSysParams(const string& s)const
					{
						map<string, map<string, string> >::const_iterator sys_params = _sparams.find(s);
						if(sys_params == _sparams.end())
							return map<string, string>();
						else
							return (*sys_params).second;
					}

					map<string, string>& getParams()const
					{ 
						return const_cast<map<string, string>&>(_params);
					}

					//auto_ptr<Server> _server;

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
					map<string, string> _pparams;
					//SParams: System params. This could be HTTP meta params, like referer, user-agent...Or something
					//else that depends on environment it executes in. Nither of these params class is guarantied to 
					//be there.
					map<string, map<string, string> > _sparams;

					ptime _timestamp;
					Type _type;

				private:			
					string _app_context;
				};
				
				class Action
				{
				public:
					virtual view::Response* handle(const Request& request)=0;

					virtual ~Action(){}
				};

				//TODO: Remove or refactor SimpleHTMLAction helper class
				template  <class T_view>
				class SimpleHTMLAction//: public Action
				{
				public:
					typedef SimpleHTMLAction<T_view > type;

					
					/*view::Response* handle(const Request& request)
					{
						empty obj1;
						return new T_view(request, obj1);
					}*/
					view::Response* handle(const Request& request)
					{
						return new T_view(request);
					}
					virtual ~SimpleHTMLAction(){}
					
				/*	static view::Response* call(const Request& request)
					{
						empty obj1;

						auto_ptr<type > view(new type()); 
						return view->handle(request, obj1);
					}*/

					class null_param{};

					template <class T1, class T2, class T3, class T4>
					view::Response* call(const Request& request, T1& obj1, T2& obj2, T3& obj3, T4& obj4)
					{
						type view; 
						return view.handle(request, obj1, obj2, obj3, obj4);
					}

					//TODO: write real overloads wtih variable num of params, for views too
					//TODO: remove "call" from action
					template <class T1, class T2, class T3>
					view::Response* call(const Request& request, T1& obj1, T2& obj2, T3 obj3)
					{
						return call(request, obj1, obj2, obj3, null_param());
					}
					template <class T1, class T2>
					view::Response* call(const Request& request, T1& obj1, T2& obj2)
					{
						return call(request, obj1, obj2, null_param(), null_param());
					}
					template <class T1>
					view::Response* call(const Request& request, T1& obj1)
					{
						return call(request, obj1, null_param(), null_param(), null_param());
					}

					view::Response* call(const Request& request)
					{
						return call(request, null_param(), null_param(), null_param(), null_param());
					}
				};
			}
		}
	}
}

#define VIEW_CLASS(x) view_##x

//TODO: Refactor DHTMLResponseclass
#define FENIX_WEB_PAGE(page_name, params)									\
class VIEW_CLASS(page_name):public view::DHTMLResponse						\
{																			\
	public: 																\
	template <class ArgumentPack>											\
	VIEW_CLASS(page_name)(ArgumentPack const& args);						\
};																			\
class page_name:public VIEW_CLASS(page_name)								\
{																			\
	public:																	\
	BOOST_PARAMETER_CONSTRUCTOR(page_name, (VIEW_CLASS(page_name)), tag, params)\
};																			\
template <class ArgumentPack> VIEW_CLASS(page_name)::VIEW_CLASS(page_name)(ArgumentPack const& args)




#define FENIX_LAYOUTED_WEB_PAGE(page_name, layout, params)					\
class VIEW_CLASS(page_name):public view::DHTMLResponse						\
{																			\
	public: 																\
	VIEW_CLASS(page_name)(){}												\
	template <class ArgumentPack>											\
	VIEW_CLASS(page_name)(ArgumentPack const& args);						\
};																			\
typedef layout<VIEW_CLASS(page_name)> layouted_page_name;					\
class page_name:public layouted_page_name									\
{																			\
		public:																\
		BOOST_PARAMETER_CONSTRUCTOR(page_name, (layouted_page_name), tag, params)\
};																			\
template <class ArgumentPack> VIEW_CLASS(page_name)::VIEW_CLASS(page_name)(ArgumentPack const& args)



#define FENIX_SIMPLE_PAGE(x)						\
class VIEW_CLASS(x):public DHTMLResponse			\
{										            \
	public:						                    \
	VIEW_CLASS(x)(const Request& request);			\
};													\
class x:public SimpleHTMLAction<VIEW_CLASS(x)>{};	\
VIEW_CLASS(x)::VIEW_CLASS(x)(const Request& request)



#define FENIX_CONTROLLER(x)														\
class x: public action::Action													\
{																				\
	public:																		\
	view::Response* handle(const action::Request& request)						\
	{return _handle(request, request.getParams());}								\
	private:																	\
	view::Response* _handle(const action::Request& request, hash& params);		\
};																				\
view::Response* x::_handle(const action::Request& request, hash& params)





