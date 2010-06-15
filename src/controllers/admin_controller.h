#pragma once

#include "fenix.h"
#include "views.h"

#include "model/models.h"

using namespace fenix::web::toolkit;

namespace PagesController
{
	namespace Admin
	{
		FENIX_CONTROLLER(index)
		{
			tables::User::obj user = this->_authenticate.get_user();
			
			if(	request.isRead()) 
			{
				if(user->role() == "super")
				{
					tables::User::collection users = user->find_all();
					
					return render_<AdminPanel::Page>((_request = request,
						_active = "none",
						_users = users,
						_time = datetime::format(request._timestamp, "%H:%M %d-%b-%Y GMT+0"), 
						_user = this->_authenticate.get_user()));
				}
				
				return render_<ForbidenResponse>();
				
			}
			
			return render_<BadRequestResponse>();
		}
		
		FENIX_CONTROLLER(activate)
		{
			tables::User::obj admin = this->_authenticate.get_user();
			
			if(admin->role() == "super")
			{
				string user_id; 
				
				if(request.isWrite() &&
					get_param(request["uid"], user_id))
				{
					tables::User::obj user = User::get(get_database(request, "fenix"));
					
					if(user->find(user_id) && !user->is_activated())
					{						
						user->activate();
						user->save();
						
						tables::Site::collection sites = user->sites<tables::Site>();
						if(!sites.empty())
						{
							Mailers::send_activation_mail(user->email(), sites[0]->id());
						}						
					}	
					
					return render_redirect("/admin/panel");					
				}
				
				return render_<BadRequestResponse>();
			}			
		
			return render_<ForbidenResponse>();
		}
	
		FENIX_CONTROLLER(impersonate)
		{
			tables::User::obj admin = this->_authenticate.get_user();
			string user_id;

			if(admin->role() == "super")
			{
				if(request.isWrite() &&
					get_param(request["uid"], user_id))
				{
					string domain = request._host[1] + "." + request._host[0];

					request.setSession("user_id", user_id, string(".") + domain);

					return render_redirect(string("http://") + domain + "/analytics/index");
					
				}
				return render_<BadRequestResponse>();
			}
			return render_<ForbidenResponse>();
		}
	}
}
