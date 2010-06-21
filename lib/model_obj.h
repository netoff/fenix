#pragma once

#include <string>
#include <map>
#include <vector>
#include <sstream>
#include "boost/shared_ptr.hpp"
#include "boost/foreach.hpp"

#define foreach BOOST_FOREACH

namespace fenix
{
	namespace web
	{
		namespace toolkit
		{
			namespace model
			{
				struct _DatabaseObj
				{
					_DatabaseObj()
					:_id(""){}
					
					std::string _id;
					std::map<std::string, std::string> _cols;
					
					bool is_empty() 
					{
						return this->_id.empty();
					}
					
					std::string to_json()
					{
						std::ostringstream out;
						
						out << "{";
						
						typedef std::pair<std::string, std::string> tuple;
						
						bool first = true;
						
						foreach(tuple p, this->_cols)
						{
							std::string a = p.first;
							std::string b = p.second;
							
							if(!first)
							{
								out <<", "; 
							}
							
							std::ostringstream val;
							
							if(a == "_id")
							{
								val << "ObjectId(\"" << b.substr(0,24) << "\")";
							}
							else
							{
								val << "\"" << b << "\"";
							}
							
							out << " " << a << ": " << val.str();
							
							first = false;
						}
						
						out << "}";
						
						return out.str();
					}
				};
				
				typedef boost::shared_ptr<_DatabaseObj> DatabaseObj;
				
				typedef std::vector<DatabaseObj> DatabaseColl;				
			}
		}
	}
}

