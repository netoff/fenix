#pragma once
#include "common.h"

using namespace std;
using namespace boost::filesystem;

const std::string supported_view_files[] = {".html",  ".htm", ".xml", ".svg"};

#define CURRENT_PATH path(".")

bool contains_dir(const std::string& dir, const path& root_path = CURRENT_PATH);
bool contains_file(const std::string& file, const path& root_path = CURRENT_PATH);
void get_files(const path& p, std::vector<path>& files);
bool is_html(const path& p);
bool is_layout(const path& p);

//void save_string_to_file(string output, string filename, string );