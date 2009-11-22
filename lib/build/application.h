#pragma once

#include "common.h"
#include "action.h"
#include "model.h"
#include "view.h"
#include "log.h"

#define APP_DATE __DATE__

namespace fenix
{
	namespace web
	{
		namespace toolkit
		{
			template <class T_dispatcher>
			class Application
			{
			public:
				Application(){}
				Application(string name)
					:_name(name){}

				static view::Response* handle(const action::Request& request)
				{
					auto_ptr<action::Action> action(new T_dispatcher());

					return action->handle(request);
				}

				~Application(){}
			private:
				string _name;

				string _date;
				
				//int _version;
				//int _build;//VERSION: ver.build

			};

#define FENIX_APPLICATION(app)								\
	extern "C" __attribute__((visibility("default"))) 		\
			view::Response* handle(const action::Request& request)	\

		}
	}
}
