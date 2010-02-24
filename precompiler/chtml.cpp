#include "chtml.h"
#include "syntax.h"
#include "page_header.h"

using namespace boost::filesystem;
using namespace std;

using namespace fenix::tools::precompile;
using namespace fenix::tools::precompile::syntax;

void read_file_to_buffer(ifstream& input, string& buf)
{
	string t_buffer;

	while(getline(input, t_buffer))
		buf.append(t_buffer + '\n');
}

void set_page_header(string input_buf, ostream& output, PageHeader& header)
{

	pair<string, string> dir;
	vector<pair<string, string> > dirs;

	rule<> header_chtml_syntax = !(header_open >> *(directive_name[assign_a(dir.first)] >> ch_p(':') >> *blank_p >>  
		directive_val[assign_a(dir.second)] >> eol_p)[push_back_a(dirs, dir)] >> header_close)
					>> (*anychar_p);

	parse_info<> result = parse(input_buf.c_str(), header_chtml_syntax);

	typedef pair<string, string> p;
	foreach(p directive, dirs)
	{
		header.set_directive(directive.first, directive.second);
	}
}

class add_code
{
public:
	add_code(ostream& of):output_file(of){};

	void operator()(const char* begin, const char* end)const
	{
		char_separator<char> sep("\n");		
		string code =string(begin, end);
		tokenizer<char_separator<char> > lines(code, sep);
		
		foreach(string line, lines)
		{
			trim(line);
			output_file << "\t" <<  line << "\n";
		}		
	}
private:
	ostream& output_file;
};

class add_echo
{
public:
	add_echo(ostream& of):output_file(of){};

	void operator()(const char* begin, const char* end)const
	{
		string code = string(begin, end);
		
		trim(code);
		output_file << "\t" << str(format("this->echo(%s);\n")%code) ;		
	}
private:
	ostream& output_file;
};

class add_text
{
public:
	add_text(ostream& of):output_file(of){};

	void operator()(const char* begin, const char* end)const
	{
		string text(begin, end);
		//escape(text);Escape after chunking

		//Break text into chunks of 1024 chs
		//Possible compiler limitations for char literals??
		int offset[] = {1024};
		offset_separator sep(offset, offset+1);
		tokenizer<offset_separator> chunks(text, sep);

		//TODO: test breaking text into chunks
		foreach(string chunk, chunks)
		{
			escape(chunk);
			output_file << "\t" << str(format("this->insert(\"%s\", false);\n")%chunk);
		}
	}
private:
	ostream& output_file;

	void escape(string& s)const
	{
		//TODO: Escape all c++ special caracters, like: \t etc.
		replace_all(s, "\\", "\\\\");
		replace_all(s, "\n", "\\n");		
		replace_all(s, "\"", "\\\"");
		replace_all(s, "\t", "\\t");
		replace_all(s, "\v", "\\v");
		replace_all(s, "\b", "\\b");
		replace_all(s, "\r", "\\r");
		replace_all(s, "\?", "\\\?");
		replace_all(s, "\'", "\\\'");
	}
};

void precompile_chtml_file(string input_buf, ostream& output)
{
	rule<> chtml_syntax = !confix_p(header_open, *anychar_p ,header_close) >>
			(*text)[add_text(output)] >> 
			*(((confix_p(echo_open, (*code)[add_echo(output)], echo_close))||
			(confix_p(code_open, (*code)[add_code(output)], code_close))) >> (*text)[add_text(output)]);
	
	parse_info<> result = parse(input_buf.c_str(), chtml_syntax);

	if(result.hit)
	{
		/*cout << "PARSED: Success" << endl
			<< "output file:" << output << endl;*/
	}
	else
	{
		throw "Failed parsing chtml file";
	}
}
void add_layout(ifstream& input, ostream& output, const string& layout)
{
	string input_buf;
	read_file_to_buffer(input, input_buf);

	output << "FENIX_PAGE_LAYOUT(" << layout << "Layout)\n";
	output << "{\n";

	precompile_chtml_file(input_buf, output);

	output << "}\n\n";
}

void add_page(ifstream& input, ostream& output, const string& page_name, set<string>& params)
{
	string input_buf;
	read_file_to_buffer(input, input_buf);

	PageHeader page_header;

	set_page_header(input_buf, output, page_header);
	
	foreach(string& param, page_header.params)
	{
		params.insert(param);
	}
	
	output << "//--------------------------------------------\n";
	output << "//Begin File: " << page_name << ".html" << endl;

	output << "namespace " << page_name << "\n{" << endl;	
	
	//Do we use layout for this page
	if(page_header.use_layout)
	{
		string layout = page_header.get_layout();

		output << "FENIX_LAYOUTED_WEB_PAGE(Page, " /*<< page_name << "Page, "*/ << layout /*<< ", " 
			<< page_header.get_required_params()*/ << ")\n";
		output << "{\n";
	}
	else
	{
		output << "FENIX_WEB_PAGE(Page, " /*<< page_name << "Page, " << page_header.get_required_params() */<< ")\n";
		output << "{\n";
	}

	precompile_chtml_file(input_buf, output);

	output << "}\n}" << endl;
	output << "//End File" << endl << endl;
}

void add_header(ofstream& output)
{
	output << "#pragma once\n" << "#include \"fenix.h\"\n\n";
	output << "#include \"model/models.h\"\n";
	output << "using namespace fenix::web::toolkit;\n";
}
