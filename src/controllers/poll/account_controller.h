#pragma once

#include "fenix.h"

using namespace fenix::web::toolkit;


namespace PagesController
{
	namespace Poll
	{
		namespace Account
		{
			FENIX_CONTROLLER(time)
			{
				if(request.isRead())
				{
					ostringstream response;
					
					string time = datetime::format(request._timestamp, "%H:%M %d-%b-%Y GMT+0");
					
					response << "$('#time').text('" << time << "');\n";
					
					return render_text(response, "text/javascript");
				}
				
				return render_<BadRequestResponse>();
			}
			
			FENIX_CONTROLLER(change_password)
			{
				string old_password, new_password, repeat_password;
				
				if(request.isWrite() && 
					get_param(request["old"], old_password) &&
					get_param(request["password"], new_password) &&
					get_param(request["repeat_password"], repeat_password) )
				{
					tables::User::obj user = this->_authenticate.get_user();
					
					ostringstream response;
					
					if(user->role() == "admin" || user->role() == "super")
					{
						user->plain_password(old_password);
						
						if(user->authenticate())
						{
							if(new_password == repeat_password)
							{
								user->set_password(new_password);
								user->save();
								
								response << "account.onPasswordChange({ok: true, msg: \"changed\"});";
							}
							else
							{
								response << "account.onPasswordChange({ok: true, msg: \"do_not_match\"});";
							}
						}
						else
						{
							response << "account.onPasswordChange({ok: false, msg: \"wrong_password\"});";
						}
					}
					else
					{
						response << "account.onPasswordChange({ok: false, msg: \"not_admin\" });";  
					}
					
					return render_text(response, "text/javascript");
				}
				
				return render_<BadRequestResponse>();
			}
			
			FENIX_CONTROLLER(add_website)
			{
				string url;
				
				if(request.isWrite() &&
					get_param(request["url"], url, validators::url()))
				{
					ostringstream response;
					
					Database::obj db = get_database(request, "fenix");
					
					User::obj user = this->_authenticate.get_user();
					
					if(user->role() == "admin" || user->role() == "super")
					{
						Site::obj site = Site::get(db);
						
						site->url(url).user(user);
						
						if(site->save())
						{
							response << "account.onAddWebsite({ok: true, msg: \"ok\"});";
						}							
						else
						{
							response << "account.onAddWebsite({ok: false, msg: \"error\"});";
						}
					}
					else
					{
						response << "account.onAddWebsite({ok: false, msg: \"not_admin\"});";
					}
					
					return render_text(response, "text/javascript");
				}	
				
				return render_<BadRequestResponse>();
			}		
			
		}
	}
}
