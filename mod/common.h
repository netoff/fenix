#pragma once

#include <memory>
#include <string>
#include <map>

#include "boost/format.hpp"
#include "boost/foreach.hpp"
#include "boost/tokenizer.hpp"
#include "boost/algorithm/string.hpp"
#include "boost/shared_ptr.hpp"

#define foreach BOOST_FOREACH

using namespace std;
using namespace boost;
using namespace boost::algorithm;

#include "config.h"


static string char2hex(char dec)
{
    char dig1 = (dec&0xF0)>>4;
    char dig2 = (dec&0x0F);
    if ( 0<= dig1 && dig1<= 9) dig1+=48;    //0,48inascii
    if (10<= dig1 && dig1<=15) dig1+=97-10; //a,97inascii
    if ( 0<= dig2 && dig2<= 9) dig2+=48;
    if (10<= dig2 && dig2<=15) dig2+=97-10;

    string r = "";
    r.append(&dig1, 1);
    r.append(&dig2, 1);
    return r;
}
inline string decode_param(const string& param)
{	
	string buffer = "";
	int len = param.length();

	for (int i = 0; i < len; i++) 
	{
		int j = i;
		char ch = param.at(j);
		if (ch == '%')
		{
			char tmpstr[] = "0x0__";
			int chnum;
			tmpstr[3] = param.at(j+1);
			tmpstr[4] = param.at(j+2);
			chnum = strtol(tmpstr, NULL, 16);
			buffer += chnum;
			i += 2;
		} 
		else
		{
			buffer += ch;
		}
	}
	
	return buffer;
}

inline string encode_param(const string& param)
{
	string ret = "";
	
	foreach(char c, param)
	{
		if(
			(48 <= c && c <= 57) ||//0-9
			(65 <= c && c <= 90) ||//abc...xyz
			(97 <= c && c <= 122) || //ABC...XYZ
			(c==' ' || c=='~' || c=='!' || c=='*' || c=='(' || c==')' || c=='\''))
		{
			ret.append(&c, 1);
		}
		else
		{
			ret.append("%");
			ret.append(char2hex(c));
		}		
	}
	
	replace_all(ret, " ", "+");
	
	return ret;
}
