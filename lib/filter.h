#pragma once

namespace fenix
{
	namespace web
	{
		namespace toolkit
		{
			namespace action
			{
				template <class T, class Ret>
				class Filter
				{
				public:
					virtual Ret operator()(const Request& request)=0;

					Filter()
						:_action()
					{
					}

					virtual ~Filter(){}
				protected:
					T _action;
				};
			}
		}
	}
}
