#pragma once

#include "model.h"
#include "table.h"

using namespace fenix::web::toolkit::model;

const string db_hostname = "localhost";
const int db_port = 1978;

const string table_db_hostname = "localhost";
const int table_db_port = 1980;

const DB table_db(table_db_hostname, table_db_port);

//forward declarations
class Site;
class User;
