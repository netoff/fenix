#pragma once

#include <string>
#include "boost/lexical_cast.hpp"
#include "boost/algorithm/string.hpp"

using namespace std;
using namespace boost;

namespace fenix
{
	namespace web
	{
		namespace toolkit
		{
			namespace action
			{
				using boost::lexical_cast;
				using boost::bad_lexical_cast;

				//template <class T>
				//bool get_param(string a, T& b);

				template <class T>
				void get_params(string a, vector<T>& b, const string& sep = ",")
				{
					vector<string> parts;
					split(parts, a, is_any_of(sep));
					foreach(string v, parts)
					{
						T val;
						if(get_param(v, val))
						{
							b.push_back(val);
						}
					}
				}

				inline bool get_param(string a, int& b)
				{

					trim(a);

					try
					{
						b = lexical_cast<int>(a);
					}
					catch(bad_lexical_cast&)
					{
						b  = 0;
						return false;
					}

					return true;
				}

				inline bool get_param(string a, float& b)
				{
					trim(a);

					try
					{
						b = lexical_cast<float>(a);
					}
					catch(bad_lexical_cast&)
					{
						b = 0.0;
						return false;
					}

					return true;
				}

				inline bool get_param(string a, double& b)
				{
					trim(a);
					
					try
					{
						b = lexical_cast<double>(a);
					}
					catch(bad_lexical_cast&)
					{
						b = 0.00;
						return false;
					}

					return true;
				}

				inline bool get_param(string s, bool& b)
				{
					trim(s);
					if(s == "1" ||  s == "t" || s == "true")
					{
						b = true;
						return true;
					}
					if(s == "0" || s == "f" || s == "false")
					{
						b = false;
						return true;
					}

					b = false;
					return false;
				}
			}
		}
	}
}
