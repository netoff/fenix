#include "file_operations.h"

bool contains_dir(const std::string& dir, const path& root_path)
{
	path p = root_path/dir;

	if(exists(p)&&is_directory(p))
		return true;
	else
		return false;
}

bool contains_file(const std::string& file, const path& root_path)
{
	path p = root_path/file;

	if(exists(p)&&!is_directory(p))
		return true;
	else
		return false;

}

void get_files(const path& p, std::vector<path>& files)
{
	directory_iterator begin(p), end;

	foreach(path p, make_pair(begin, end))
	{
		if(!is_directory(p))
			files.push_back(p);
	}
}

bool _is_view_file(const string& extension)
{
	foreach(string ext, supported_view_files)
	{
		if(ext == extension)
			return true;
	}
	return false;
}

bool is_html(const path& p)
{
	if(!is_directory(p) && _is_view_file(p.extension()))
		return true;
	else
		return false;
}
bool is_layout(const path& p)
{
	if(p.filename() == "layout.html")
		return true;
	else
		return false;
}
