#pragma once

#include "boost/tokenizer.hpp"

#include "boost/mpl/vector.hpp"
#include "boost/mpl/push_back.hpp"
#include "boost/mpl/for_each.hpp"
#include "boost/mpl/front.hpp"
#include "boost/proto/proto.hpp"

#include "common.h"
#include "action.h"
#include "view.h"
#include "layout.h"

#include "exception.h"

using namespace fenix::web::toolkit::view;
using namespace boost;

#define _(x) proto::lit(x)

namespace fenix
{
	namespace web
	{
		namespace toolkit
		{
			namespace action
			{
				FENIX_SIMPLE_PAGE(DispatcherExceptionPage)
				{
					insert(_$(H1(Dispatcher Exception)));
					insert(_$(Page can not be found!));
				}

				FENIX_CONTROLLER(ExceptionAction)
				{
					DispatcherExceptionPage page;
					return page.handle(request);
				}

				template <class T_action>
				struct _action
				{
					friend std::ostream& operator<<(std::ostream& stream, _action<T_action>)
					{
						stream << "Action";
						return stream;
					}
					typedef T_action type;
				};

				struct _param
				{
					string& _p;

					_param(string& p)
						:_p(p){}
				};
				typedef _param _p;
				
				struct _matcher
				{
					_matcher(){}

					virtual auto_ptr<Action> get_action()
					{
						auto_ptr<Action> action(new ExceptionAction);
						return action;
					}
					virtual ~_matcher(){}

					vector<string> _pattern;
					void add_pattern(string s){_pattern.push_back(s);}

					bool operator()(const Request& req){return _match(req);}
					bool _match(const Request& req)
					{
						bool match = false;

						if(_pattern.size() != req._query.size())
							return false;
						int i = 0;
						foreach(string s, _pattern)
						{
							if( i >= req._query.size())
								return false;
							if(s != req._query[i++])
								return false;
						}
						return true;
					}
				};

				template <class T>
				struct matcher:public _matcher
				{

					matcher(){}
					auto_ptr<Action> get_action()
					{
						auto_ptr<Action> action(new T());
						return action;
					}

				};

				struct dispatcher_context:
					public proto::context::callable_context<dispatcher_context>
				{
					const Request& _req;
					typedef auto_ptr<_matcher> result_type;


					dispatcher_context(const Request& request)
						:_req(request){}

					auto_ptr<_matcher> operator()(proto::tag::terminal, char const* str)
					{
						auto_ptr<_matcher> m(new _matcher());
						m->add_pattern(string(str));
					    return m;
					}

					template <class T>
					auto_ptr<_matcher> operator()(proto::tag::terminal, _action<T>)
					{
						auto_ptr<_matcher> m(new matcher<T>());
						return m;
					}

			
					template <class Expr1, class Expr2>
					auto_ptr<_matcher> operator()(proto::tag::divides, Expr1 left, Expr2  right)
					{
						auto_ptr<_matcher> a = proto::eval(left, *this);
						auto_ptr<_matcher> b = proto::eval(right, *this);
						if(!b->_pattern.empty())
							a->add_pattern(b->_pattern[0]);

						return a;
					}				
					
					template <class Expr1, class Expr2>
					auto_ptr<_matcher> operator()(proto::tag::shift_left, Expr1 left, Expr2 right)
					{
						auto_ptr<_matcher> pat = proto::eval(left, *this);
						auto_ptr<_matcher> m = proto::eval(right, *this);
						m->_pattern = pat->_pattern;
			
						return m;
					}

					template <class Expr1, class Expr2>
					auto_ptr<_matcher> operator()(proto::tag::logical_or, Expr1 left, Expr2 right)
					{
						auto_ptr<_matcher> a = proto::eval(left, *this);

						if((*a)(_req))
							return a;
						else 
						{
							auto_ptr<_matcher> b = proto::eval(right, *this);

							if((*b)(_req))
								return b;
						}

						auto_ptr<_matcher> m(new _matcher());
						return m;
							//else
							//{
							//	//TODO put this to dispatch funct
							//	_matcher e(new DispatcherException());
							//	return e;
							//}
					}

				};
				
				template <class Expr>
				auto_ptr<Action> dispatch(Expr& expr, const Request& request)
				{
					dispatcher_context context(request);

					return proto::eval(expr, context)->get_action();
				}
				
			}
		}
	}
}
