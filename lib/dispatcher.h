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
				//No action
				struct empty{};
				
				FENIX_CONTROLLER(not_found)
				{
					return render_<NotFoundResponse>();
				}

				template <template<class> class T_action, class Authenticate = empty_class>
				struct _action
				{				
					typedef T_action<Authenticate> Type;
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
					
					_matcher(const string& s)
					{
						_pattern.push_back(s);
					}

					virtual auto_ptr<Base> get_action() = 0;
					
					virtual ~_matcher(){}
					
					vector<string>& pattern() { return _pattern; }
					void add_pattern(string s) { _pattern.push_back(s); }

					bool operator()(const Request& req) { return _match(req); }					
					
				protected:					
					vector<string> _pattern;
				private:
					bool _match(const Request& req) const
					{
						if(_pattern.size() != req._query.size())
						{
							return false;
						}
						
						int i = 0;
						
						foreach(string s, _pattern)
						{
							if( i >= req._query.size())
							{
								return false;
							}
							if(s != req._query[i++])
							{								
								return false;
							}
						}
						
						return true;
					}
				};

				template <class T=empty>
				struct matcher:public _matcher
				{

					matcher(){}
					
					matcher(const string& s)
					:_matcher(s)
					{
					}
					
					auto_ptr<Base> get_action()
					{
						auto_ptr<Base> action(new T());
						
						return action;
					}

				};
				
				template<>
				struct matcher<empty>:public _matcher
				{
					matcher(){}
					
					matcher(const string& s)
					:_matcher(s)
					{
					}
					
					auto_ptr<Base> get_action()
					{
						auto_ptr<Base> action(new not_found<>());
						
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
						auto_ptr<_matcher> m(new matcher<>(string(str)));
						
						return m;
					}

					template <template<class> class T, class A>
					auto_ptr<_matcher> operator()(proto::tag::terminal, _action<T, A>)
					{
						auto_ptr<_matcher> m(new matcher<T<A> >());
						
						return m;
					}

			
					template <class Expr1, class Expr2>
					auto_ptr<_matcher> operator()(proto::tag::divides, Expr1 left, Expr2  right)
					{
						auto_ptr<_matcher> a = proto::eval(left, *this);
						auto_ptr<_matcher> b = proto::eval(right, *this);
						
						if(!b->pattern().empty())
						{
							a->add_pattern(b->pattern()[0]);
						}

						return a;
					}				
					
					template <class Expr1, class Expr2>
					auto_ptr<_matcher> operator()(proto::tag::shift_left, Expr1 left, Expr2 right)
					{
						auto_ptr<_matcher> pat = proto::eval(left, *this);
						auto_ptr<_matcher> m = proto::eval(right, *this);
						
						m->pattern().assign(pat->pattern().begin(), pat->pattern().end());
			
						return m;
					}

					template <class Expr1, class Expr2>
					auto_ptr<_matcher> operator()(proto::tag::logical_or, Expr1 left, Expr2 right)
					{
						auto_ptr<_matcher> a = proto::eval(left, *this);

						if((*a)(_req))
						{
							return a;
						}
						else 
						{
							auto_ptr<_matcher> b = proto::eval(right, *this);

							if((*b)(_req))
							{
								return b;
							}
						}

						//Empty action => dispatcher exception
						auto_ptr<_matcher> m(new matcher<>());
						
						return m;
					}

				};
				
				template <class Expr>
				auto_ptr<Base> dispatch(Expr& expr, const Request& request)
				{
					dispatcher_context context(request);

					return proto::eval(expr, context)->get_action();
				}
				
			}
		}
	}
}
