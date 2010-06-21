#pragma once

#include "fenix.h"
#include "views.h"
//#include "model/models.h"

#include "middleware.h"

using namespace fenix::web::toolkit;
using namespace pages;

namespace DebugController
{
	FENIX_CONTROLLER(db)
	{
		string database, query;
		
		if(request.isWrite())
		{	
			if(
				//get_param(request["db"], database) &&
				get_param(request["query"], query))
			{
				ScopedMiddleware mid;
				
				string result = mid.query(query);
				
				mid.done();
				
				return render_<QueryDebug::Page>((_request = request, _output = result, _database = database, _query = query));
			}
			
			return render_<BadRequestResponse>();
		}
		
		return render_<QueryDebug::Page>((_request = request));
	}
	
	/*
	FENIX_CONTROLLER(timestamp)
	{
	}*/
}
