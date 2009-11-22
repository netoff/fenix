#pragma once

#include <memory>
#include <string>
#include <iostream>
#include <fstream>

#include <vector>
#include <utility>
#include <map>
#include <list>

using namespace std;

#include "boost/format.hpp"
#include "boost/cstdint.hpp"
#include "boost/shared_ptr.hpp"
#include "boost/foreach.hpp"

using namespace boost;

#define foreach BOOST_FOREACH
typedef map<string, string> hash;
