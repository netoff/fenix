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
		model::Database db("localhost", 1978);

		string response = "";
		long t = 0;
		long timestamp = datetime::timestamp(request._timestamp);

		if(get_param(params["t"], t))
		{
			if(t == 0) t = timestamp - 1;

			if(t < timestamp)
			{
				string val = str(format("%i")%t);
				string points = db.ext("get_stat", "s:x123:vps", val);

				for(++t; t < timestamp; t++)
				{
					string val = str(format("%i")%t);
					string new_point = db.ext("get_stat", "s:x123:vps", val);
					points += str(format(", %s")%new_point);
				}
				response =  str(format("updatePoints([%s]);\ntimestamp = %i;")%points%(timestamp - 1));
				response +=
					"drawChart();\n"
					"setTimeout('updateDashboard()', 500);";
			}
			else
			{
				response += "setTimeout('updateDashboard()', 500);";
			}
		}
		else
		{
			return new BadRequestResponse();
		}

		return new InlineResponse(response, "text/javascript");
	}
}
