#pragma once

#include "fenix.h"
#include "views.h"

#include <ctime>
#include <boost/random.hpp>

using namespace boost;

using namespace fenix::web::toolkit;
using namespace pages;

namespace StatsController
{
	FENIX_CONTROLLER(views)
	{

		mt19937 rng;
		rng.seed(static_cast<unsigned int>(time(0)));

		uniform_int<> ten(0,10);

		variate_generator<mt19937&, uniform_int<> > stats(rng, ten);

		int vps = stats();

		//model::Database db("localhost", 1978);
		//int vps = db.get<int>("s:1234:vps");
		//string vps = db.get("s:1234:vps");

		//return new Stats::Page(Stats::_request = request, Stats::_vps = vps);

		string response =  str(format("updatePoints([%i]);\n")%vps);

		response +=
				"drawChart();\n"
				"setTimeout('updateDashboard()', 500);";

		return new InlineResponse(response, "text/javascript");
	}
}
