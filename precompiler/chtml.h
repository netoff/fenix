#pragma once

#include "common.h"

void precompile_chtml_file(std::ifstream& input, std::ofstream& output);
void add_header(std::ofstream& output);
void add_layout(std::ifstream& input, std::ostream& output, const std::string& layout = "Application");
void add_page(std::ifstream& input, std::ostream& output, const std::string& page_name, std::set<std::string>& params);