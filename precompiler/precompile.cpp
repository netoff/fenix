#include "common.h"
#include "chtml.h"
#include "file_operations.h"

const string views_folder = "views";
const string layouts_folder = "views/layouts";
const string output_filename = "src/views.h";

using namespace std;
using namespace boost::filesystem;

int main(int argc, char** argv)
{
	//determine application name
	//TODO: application name

	ofstream output_file(output_filename.c_str());

	if(!contains_dir(views_folder))
	{
		cout << "Error: " << "Directory does not exist: (views)!" << endl;
		return 1;
	}
	else
	{
		path views(views_folder);
		views = complete(views);

		add_header(output_file);

		output_file << "namespace pages" << endl
			<< "{" << endl ;
 
		set<string> params;
		
		ostringstream _out;
		
		//TODO: Enable subfolders in views folder(one level?)
		//TODO: Put subfolder views to namespace same as folder name
		path layouts(layouts_folder);
		layouts = complete(layouts);

		vector<path> layout_files;
		get_files(layouts, layout_files);
		foreach(path p, layout_files)
		{
			if(is_html(p))
			{				
 				cout << "Processing Layout file: " << p.filename() << endl; 

				ifstream input_file(p.file_string().c_str());

				string layout_name = p.stem();

				add_layout(input_file, _out, layout_name);
			}
		}

		vector<path> view_files;
		get_files(views, view_files);		

		foreach(path p, view_files)
		{
			if(is_html(p) && !is_layout(p))
			{				
 				cout << "Processing file: " << p.filename() << endl; 

				ifstream input_file(p.file_string().c_str());

				string page_name = p.stem();

				add_page(input_file, _out, page_name, params);
			}
		}
		
		output_file << "\n\n";
		
		foreach(string param, params)
		{
			output_file << "BOOST_PARAMETER_NAME(" << param << ");\n";	
		}
		
		output_file << "\n\n" << _out.str();
	}

	output_file << "}" << endl ;

	output_file.close();

	return 0;
}
