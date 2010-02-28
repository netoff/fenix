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
			//if user is already loged in, go to the user dashboard
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
				
				if(	get_param(request["email"], email, validators::email()) && 
					get_param(request["password"], password) && 
					request.isWrite() )
				{
					shared_ptr<tables::User> user(new tables::User(get_users_db(request)));
					
					if(user->find(model::Query(get_users_db(request)).add_string("email", email).add_limit(1)))
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
			}
			else
			{
				return render_redirect("/app/index");
			}
			
			return render_<BadRequestResponse>();
		}
		
		//======================
		//Logout
		//======================
		FENIX_CONTROLLER(clear)
		{
			if(request.isRead())
			{
				string user_id = request["user_id"];
				
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
					get_param(request["email"], email, validators::email()) &&
					get_param(request["password"], password) &&
					get_param(request["repeat_password"], repeat_password) &&
					get_param(request["url"], url, validators::url()) && /*&&
					get_param(params["timezone"], timezone)*/
					request.isWrite())
				{
					if(password == repeat_password)
					{
						shared_ptr<tables::User> user(new tables::User(get_users_db(request)));
						
						if(!user->exists(model::Query(get_users_db(request)).add_string("email", email)))
						{
							shared_ptr<tables::Site> site(new tables::Site(get_users_db(request)));
							
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
							return render_<Login::Page>((_request = request, _signup_error = "Email address already in use.", _url = url));
						}
					}
					
					return render_<Login::Page>((_request = request, _signup_error = "Some error occured, please try again."));
				}
			}
			
			return render_<BadRequestResponse>();			
		}
		
		FENIX_CONTROLLER(create_finish)
		{
			return render_<Login::Page>((_request = request, 
				_alert = "<p>Thank you for a sign up.<br /><br /> We are currently in a limited beta. As soon as more "
				"slots become available, we will send you activation link, "
				"so you can verify your email address and start using our service.</p>"));
		}
	}
}
