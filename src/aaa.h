#pragma once

#include "fenix.h"
#include "model/models.h"

using namespace fenix::web::toolkit;

namespace session_authentication
{
	static const string login_page = "/app/login";
	
	class authenticate
	{
	public:
		authenticate()
		:_redirect(login_page),_user(new tables::User(table_db)){}
		
		bool operator()(const action::Request& request, string& redirect)
		{
			string user_id = request.getSession("user_id");
			
			if(!user_id.empty())
			{
				if(_user->find(user_id))
				{
					return true;
				}
			}
			
			redirect = _redirect;
			return false;
		}
		
		shared_ptr<tables::User> get_user()
		{
			return _user;
		}
	private:
		string _redirect;
		shared_ptr<tables::User> _user;
	};
}
