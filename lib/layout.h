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
				class DHTMLLayout:public T, private noncopyable
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
					
					virtual ~DHTMLLayout(){}
				protected:
					void applay_layout(){T::insert(_buffer);}
				private:
					string _buffer;
				};
			}
		}
	}
}

#define FENIX_PAGE_LAYOUT(layout_name)						\
template <class T>								\
class layout_name: public view::DHTMLLayout<T>, private noncopyable\
{										\
	template <class ArgumentPack>						\
	void _render(const ArgumentPack& args);					\
public:										\
	layout_name(){}					\
	virtual ~layout_name(){}\
	template <class ArgumentPack>						\
	layout_name(const ArgumentPack& args)					\
	:view::DHTMLLayout<T>(args)						\
	{									\
		_render(args);							\
	}									\
};										\
template <class T> template<class ArgumentPack> void layout_name<T>::_render(const ArgumentPack& args)

