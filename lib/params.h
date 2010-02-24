#pragma once

#include <string>
#include "boost/lexical_cast.hpp"
#include "boost/algorithm/string.hpp"
#include "boost/xpressive/xpressive.hpp"

using namespace std;
using namespace boost;
using namespace boost::xpressive;

namespace fenix
{
	namespace web
	{
		namespace toolkit
		{
			namespace action
			{
				namespace validators
				{
					struct email{};
					struct url{};
				}
				
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

				inline bool get_param(const string& a, int& b)
				{

					string s = trim_copy(a);

					try
					{
						b = lexical_cast<int>(s);
					}
					catch(bad_lexical_cast&)
					{
						b  = 0;
						return false;
					}

					return true;
				}
				
				inline bool get_param(const string& a, int64_t& b)
				{
					string s = trim_copy(a);
					 
					try
					{
						b = lexical_cast<int64_t>(s);
						
						return true;
					}
					catch(bad_lexical_cast&)
					{
						b = 0;
					}
					
					return false;					
				}
				
				inline bool get_param(const string& a, long& b)
				{
					string s = trim_copy(a);

					try
					{
						b = lexical_cast<long>(s);
					}
					catch(bad_lexical_cast&)
					{
						b  = 0;
						return false;
					}

					return true;
				}

				inline bool get_param(const string& a, float& b)
				{
					string s = trim_copy(a);

					try
					{
						b = lexical_cast<float>(s);
					}
					catch(bad_lexical_cast&)
					{
						b = 0.0;
						return false;
					}

					return true;
				}

				inline bool get_param(const string& a, double& b)
				{
					string s = trim_copy(a);
					
					try
					{
						b = lexical_cast<double>(s);
					}
					catch(bad_lexical_cast&)
					{
						b = 0.00;
						return false;
					}

					return true;
				}

				inline bool get_param(const string& a, bool& b)
				{
					string s = trim_copy(a);

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
				
				inline bool get_param(const string& a, string& b)
				{
					//for now only trim string 
					//later we could slice it, escape it, etc..
					b = trim_copy(a);
					
					if(!b.empty())
					{
						return true;
					}
					
					return false;
				}
				
				template <class T>
				inline bool get_param(const string& a, string& b, T& t);
				
				template <>
				inline bool get_param<validators::email>(const string& a, string& b, validators::email& v)
				{
					sregex pattern = sregex::compile("^[a-z0-9._%+-]+@[a-z0-9.-]+\.[a-z]{2,4}$", regex_constants::icase);
					
					if(regex_match(a, pattern))
					{
						return get_param(a, b);
					}
				}
				
				template <>
				inline bool get_param<validators::url>(const string& a, string& b, validators::url& v)
				{
					sregex pattern = sregex::compile("^http(s)?://([\\w+?\\.\\w+])+([a-zA-Z0-9\\~\\!\\@\\#\\$\\%\\^\\&amp;\\*\\(\\)_\\-\\=\\+\\\/\?\.\:\;\'\,]*)?$", regex_constants::icase);
					
					if(regex_match(a, pattern))
					{
						return get_param(a, v);
					}
				}
				
				
			}
		}
	}
}
