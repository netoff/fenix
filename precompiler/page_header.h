#pragma once

#include <string>
#include <vector>

using namespace std;


namespace fenix
{
	namespace tools
	{
		namespace precompile
		{
			struct PageHeader
			{
				bool			use_layout;
				string			layout;
				string			mime_type;
				//string			_class;
				vector<string>	includes;
				vector<string>	params;
				vector<string>	required_params;

				PageHeader()
					:use_layout(true),layout("App"),
					mime_type("text")
				{
					params.push_back("request");
				}

				string get_required_params()
				{
					string val = "";
					val += "(required ";
					foreach(string p, params)
					{
						val += "(" + p + ", *) ";
					}

					val += ")";

					return val;
				}

				string get_optional_params()
				{
					return "";
				}

				string get_layout()
				{
					if(use_layout)
					{
						if(layout != "" && layout != " ")
						{
							return layout + "Layout";
						}
					}
					return "";
				}

				void set_directive(const string& dir, string val)
				{
					if(dir == "use-layout")
					{
						use_layout = (val == "false") ? false : true;
						return;
					}
					if(dir == "layout")
					{
						layout = val;
						return;
					}
					if(dir == "mime-type")
					{
						mime_type = val;
						return;
					}
					if(dir == "include")
					{
						includes.push_back(val);
						return;
					}
					if(dir == "param")
					{
						params.push_back(val);
						return;
					}
				}
			};
		}
	}
}