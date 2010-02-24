#pragma once

#include <fenix.h>

#include "views.h"
#include "model/models.h"

using namespace fenix::web::toolkit;
using namespace pages;

namespace LoginController
{
	FENIX_CONTROLLER(index)
	{
		string user_id = request.getSession("user_id");
		
		if(!user_id.empty())
		{
			return render_redirect("/app/index"); 
		}
		
		return render_<Login::Page>(_request = request);
	}
	
	namespace session
	{
		//======================
		//Login
		//======================
		FENIX_CONTROLLER(create)
		{
			if(request.getSession("user_id").empty())
			{
				string email, password;
				
				if(get_param(params["email"], email) && get_param(params["password"], password) && request.isWrite())
				{
					shared_ptr<tables::User> user(new tables::User(table_db));
					
					if(user->find(model::Query(table_db).add_string("email", email).add_limit(1)))
					{
						user->plain_password(password);
						
						if(user->authenticate() && user->activated())
						{
							request.setSession("user_id", user->id_s()); 
							
							return render_redirect("/app/index");
						}
					}
					
					return render_<Login::Page>((_request = request, _email = email, 
						_login_error = "Wrong email address or password. Please try again."));
				}
				
				return render_<BadRequestResponse>();
			}
			else
			{
				return render_redirect("/app/index");
			}
		}
		
		//======================
		//Logout
		//======================
		FENIX_CONTROLLER(clear)
		{
			if(request.isRead())
			{
				string user_id = request.getParam("user_id");
				
				request.clearSession();
				
				return render_redirect("/app/login");
			}
			
			return render_<BadRequestResponse>();
		}
	}
	
	namespace account
	{	//==========================
		//Sign Up
		//=========================
		//create new user account		
		FENIX_CONTROLLER(create)
		{
			if(request.isWrite())
			{
				string email, password, repeat_password, url, timezone;
				
				if(
					get_param(params["email"], email) &&
					get_param(params["password"], password) &&
					get_param(params["repeat_password"], repeat_password) &&
					get_param(params["url"], url) && /*&&
					get_param(params["timezone"], timezone)*/
					request.isWrite())
				{
					if(password == repeat_password)
					{
						shared_ptr<tables::User> user(new tables::User(table_db));
						
						if(!user->exists(model::Query(table_db).add_string("email", email)))
						{
							shared_ptr<tables::Site> site(new tables::Site(table_db));
							
							user->email(email).plain_password(password);
							site->url(url).user(user);	
							
							//cascade save
							if(site->save())
							{ 								
								return render_redirect("/app/signup_thanks");
							}
						}
						else
						{
							return render_<Login::Page>((_request = request, _signup_error = "Email already exists.", _url = url));
						}
					}
					
					return render_<Login::Page>((_request = request, _signup_error = "Some error occured, please try again."));
				}
			}
			
			return render_<BadRequestResponse>();			
		}
	}
}
