#pragma once

#include "fenix.h"

using namespace fenix::web::toolkit;

namespace Mailers
{
	inline void send_registration_mail(const string& email)
	{
		ScopedMiddleware mid;
		
		ostringstream query;
		query << "send_registration_mail([[" << email << "]])";
		
		mid.insert(query.str());
		
		mid.done();
	}
	
	inline void send_activation_mail(const string& email, const string& site_id)
	{
		ScopedMiddleware mid;
		
		ostringstream query;
		query << "send_activation_mail([[" << email << "]], [[" << site_id << "]])";
		
		mid.insert(query.str());
		
		mid.done();
	}
}
