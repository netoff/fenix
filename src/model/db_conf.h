#pragma once

#include "fenix.h"

#include "model.h"
#include "table.h"

using namespace fenix::web::toolkit;
using namespace fenix::web::toolkit::model;

Database::obj get_database(const action::Request& request, const string& db)
{
	string hostname = request("config", "db_host");
	int port = action::to_integer(request("config", "db_port"));
	
	Database::obj ret(new Database(hostname, db, port));
	
	/*request.add_note(Notifier<Database::obj>::get(ret));*/
	
	return ret;	
}

namespace tables
{
//forward declarations
class Site;
class User;
}

const int SITE_ID_LENGTH = 24;
