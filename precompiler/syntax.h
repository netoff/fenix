#pragma once
#include "boost/spirit/core.hpp"
#include "boost/spirit/actor/increment_actor.hpp"
#include "boost/spirit/utility/confix.hpp"
#include "boost/algorithm/string.hpp"
#include "boost/tokenizer.hpp"

using namespace boost;
using namespace boost::spirit;

namespace fenix
{
	namespace tools
	{
		namespace precompile
		{
			namespace syntax
			{
				//chtml syntax
				rule<> echo_open = str_p("<!--@=");
				rule<> echo_close = str_p("-->");

				rule<> code_open = str_p("<!--@");
				rule<> code_close = str_p("-->");

				rule<> code = anychar_p;
				rule<> text = anychar_p - code_open;

				//HEADER precompiler directive
				rule<> header_open = str_p("<!--@HEADER@") >> eol_p;
				rule<> header_close = str_p("-->");

				rule<> directive_name = lower_p >> *(!(ch_p('_')|ch_p('-')) >> lower_p);
				rule<> directive_val = *(anychar_p - (eol_p | header_close));
			}
		}
	}
}