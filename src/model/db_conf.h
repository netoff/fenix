#pragma once

#include "fenix.h"

#include "model.h"
#include "table.h"

using namespace fenix::web::toolkit;
using namespace fenix::web::toolkit::model;

DB get_users_db(const action::Request& request)
{
	string hostname = request("config", "users_db_host");
	int port = action::to_integer(request("config", "users_db_port"));
	
	return DB(hostname, port);
}

DB get_log_db(const action::Request& request)
{
	string hostname = request("config", "log_db_host");
	int port = action::to_integer(request("config", "log_db_port"));
	
	return DB(hostname, port);
}

//forward declarations
class Site;
class User;
