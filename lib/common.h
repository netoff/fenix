#pragma once

#include <memory>
#include <string>
#include <iostream>
#include <fstream>

#include <vector>
#include <utility>
#include <map>
#include <list>
#include <deque>
#include <set>

#include <stdint.h>

using namespace std;

#include "boost/format.hpp"
#include "boost/cstdint.hpp"
#include "boost/shared_ptr.hpp"
#include "boost/foreach.hpp"
#include "boost/shared_ptr.hpp"
#include "boost/algorithm/string.hpp"
#include "boost/utility.hpp"

using namespace boost;
using namespace boost::algorithm;

#define foreach BOOST_FOREACH
typedef map<string, string> hash;

class empty_class{};
