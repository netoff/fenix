#pragma once

#include "fenix.h"
#include "model/models.h"

using namespace fenix::web::toolkit;

namespace session_authentication
{
	static const string login_page = "/" + string(ROOT_PATH) + "/login";
	
	template <int LOG>
	class authenticate
	{
	public:
		authenticate(){}
		
		authenticate(const action::Request& request)
		:_redirect(login_page),_db(get_database(request, "fenix")),
		_user(tables::User::get(_db)){}
		
		bool operator()(const action::Request& request, string& redirect)
		{
			string user_id = request.getSession("user_id");
			
			if(!user_id.empty() && _user.get())
			{
				if(_user->find(user_id))
				{
					if(LOG)
					{
						this->_user->log_activity();
						this->_user->save();
					}
					
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
		
		bool authorize_user(const string& site_id)
		{
			if(site_id.size() == SITE_ID_LENGTH)
			{
				tables::Site::obj site = tables::Site::get(this->_db);
				
				return site->exists(Query()
						.add_cond("_id", site_id)
						.add_cond("user_id", this->_user->id()));
			}
			
			return false;
		}
	private:
		string _redirect;
		
		Database::obj _db;
		tables::User::obj _user;
	};
}
