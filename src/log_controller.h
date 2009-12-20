#pragma once

#include <fenix.h>

namespace LogController
{
	FENIX_CONTROLLER(hit)
	{
		//get_params<string>
		string site_id = params["s"];

		//int timestamp = t & 0x03;

		string server_time = to_simple_string(request._timestamp);
		long timestamp = datetime::timestamp(request._timestamp);

		//Circular buffer with 8 elements
		long t = timestamp & 7;

		string key = str(format("s:%s")%site_id);
		string val = str(format("%i=vps:%i")%timestamp%t);

		model::Database db("localhost", 1978);
		string n = db.ext("incr", key, val);

		string response = str(format("%s:%s")%t%n);

		return new InlineResponse(response, "text/plain");
	}
}
