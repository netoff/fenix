#pragma once

#include "action.h"

using namespace fenix::web::toolkit::action;

namespace fenix
{
	namespace web
	{
		namespace toolkit
		{
			namespace view
			{
				template <class T>
				class DHTMLLayout:public T
				{
				public:
					DHTMLLayout()
					{

					}
					template<class ArgumentPack>
					DHTMLLayout(ArgumentPack const& args)
						:T(args)
					{
						_buffer = T::_response;
						T::_response.clear();
					};

				protected:
					//void insert(string s){T::insert(s);}

					//template <class T_param1>
					//void insert(string s, T_param1 param){T::insert(s, param);}

					//template <class T_param1, class T_param2>
					//void insert(string s, T_param1 param1, T_param2 param2){T::insert(s, param1, param2);}

					//template <class T_param1, class T_param2, class T_param3>
					//void insert(string s, T_param1 param1, T_param2 param2, T_param3 param3){T::insert(s, param1, param2, param3);}

					//void echo(string a){insert(escape(a), false);}

					void applay_layout(){T::insert(_buffer);}
				private:
					string _buffer;
				};
			}
		}
	}
}

#define FENIX_PAGE_LAYOUT(layout_name)			\
template <class T>								\
class layout_name: public view::DHTMLLayout<T>	\
{												\
	void _render();								\
public:                                     	\
	template <class ArgumentPack>				\
	layout_name(ArgumentPack const& args)		\
		:view::DHTMLLayout<T>(args)				\
    {											\
		_render();								\
    }											\
};												\
template <class T> 								\
void layout_name<T>::_render()

