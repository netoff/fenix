#include "common.h"
#include "chtml.h"
#include "file_operations.h"

#define VIEWS_FOLDER "views"
#define LAYOUTS_FOLDER "views/layouts"
#define OUTPUT_FILENAME "src/views.h"

using namespace std;
using namespace boost::filesystem;

int main(int argc, char** argv)
{
	//determine application name
	//TODO: application name

	ofstream output_file(OUTPUT_FILENAME);

	if(!contains_dir(VIEWS_FOLDER))
	{
		cout << "Error: " << "Directory does not exist: (views)!" << endl;
		return 1;
	}
	else
	{
		path views(VIEWS_FOLDER);
		views = complete(views);

		add_header(output_file);

		output_file << "namespace pages" << endl
			<< "{" << endl ;
 
		//TODO: Enable subfolders in views folder(one level?)
		//TODO: Put subfolder views to namespace same as folder name

		path layouts(LAYOUTS_FOLDER);
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

				add_layout(input_file, output_file, layout_name);
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

				add_page(input_file, output_file, page_name);
			}
		}
	}

	output_file << "}" << endl ;

	output_file.close();

	return 0;
}
