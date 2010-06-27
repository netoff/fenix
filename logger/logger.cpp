#include <common.h>
#include <fstream>
#include "logger.h"

#include "log.h"

#include "engine_pool.h"

using namespace fenix::misc::server;

static const string stored_file = "/opt/lua/ext/share/lua/5.1/ext.lua";

string read_stored()
{
	string ret = "";
	
	ifstream stored_stream(stored_file.c_str());
		
	string buf = "";
	while(getline(stored_stream, buf))
	{
		ret.append(buf + '\n');
	}
	stored_stream.close();
	
	return ret;
}

int main()
{
	try
	{		
		//read stored proc file
		string stored_file = "";
		stored_file = read_stored();
		
		engine_pool.set_stored(stored_file);
		
		unlink(local_endpoint.c_str());
		logger_server server;
		
		server.run();		
		//io_service.run();
	}
	catch(std::exception& e)
	{
		Log().get() << "Error: " << e.what() << endl;
	}
	
	return 0;
}
