#pragma once

#include <fenix.h>

#include "views.h"
#include "model/models.h"
#include "mailers/mailers.h"

using namespace fenix::web::toolkit;
using namespace pages;

using namespace tables;

static const int RESET_TOKEN_SIZE = 16;

namespace LoginController
{
	static string get_domain(const action::Request& request)
	{
		return "." + request._host[1] + "." + request._host[0];
	}
	
	FENIX_CONTROLLER(index)
	{
		if(request.isRead())
		{
			string user_id = request.getSession("user_id");
			
			if(!user_id.empty())
			{
				//if user is already loged in, go to the user dashboard
				return render_redirect("/" + string(ROOT_PATH), "/index"); 
			}
			
			bool finish = false;
			bool reset_password = false;
			bool password_reset = false;
			bool reset_error = false;
			//bool login_error = false;
			
			get_param(request["signup_finish"], finish);
			get_param(request["reset_password"], reset_password);
			get_param(request["password_reset"], password_reset);
			get_param(request["reset_error"], reset_error);
			//get_param(request["login_error"], login_error);
			
			
			return render_<Login::Page>((_request = request, _finish = finish, 
				_reset_password = reset_password, _password_reset = password_reset,// _login_error = login_error,
				_reset_error = reset_error));
		}
		
		return render_<BadRequestResponse>();
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
					Database::obj db = get_database(request, "fenix");
					
					User::obj user = User::get(db);					
					
					if(user->find(model::Query().add_cond("email", email)))
					{
						user->plain_password(password);
						
						if(user->authenticate() && user->activated())
						{							
							request.setSession("user_id", user->id(), get_domain(request), 24*15);
							
							//record login time
							user->log_login();
							user->save();
							
							return render_redirect("/" + string(ROOT_PATH), "/index");
						}
					}
					
					return render_<Login::Page>((_request = request, _email = email, 
						_login_error = "Wrong email address or password. Please try again."));
				}				
			}
			else
			{
				return render_redirect("/" + string(ROOT_PATH), "/index");
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
				
				request.clearSession(get_domain(request));
				
				return render_redirect("/" + string(ROOT_PATH), "/login");
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
						Database::obj db = get_database(request, "fenix");
						
						User::obj user = User::get(db);
						
						if(!user->exists(model::Query().add_cond("email", email)))
						{
							Site::obj site = Site::get(db);
							
							user->email(email).set_password(password);
							site->url(url).user(user);	
							
							//cascade save
							if(site->save())
							{
								Mailers::send_registration_mail(user->email());
 								
								return render_redirect("/" + string(ROOT_PATH), "/login?signup_finish=1");
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
		
		FENIX_CONTROLLER(forgot)
		{
			string email;
			
			if(request.isWrite() &&
				get_param(request["email"], email, validators::email()))
			{
					Database::obj db = get_database(request, "fenix");
					
					User::obj user = User::get(db);					
					
					if(user->find(model::Query().add_cond("email", email)))
					{
						string token = crypto::random<RESET_TOKEN_SIZE>();
						
						user->token(token);
						
						if(user->save())
						{
							Mailers::send_reset_mail(user->email(), user->token());
 								
							return render_redirect("/" + string(ROOT_PATH), "/login?password_reset=1");
						}
						else
						{
							return render_redirect("/" + string(ROOT_PATH), "/login?reset_password=1&reset_error=1");
						} 
					}				
			}
			
			return render_<BadRequestResponse>();
		}
		
		FENIX_CONTROLLER(reset)
		{
			string token, password, repeat_password;
			
			if(get_param(request["t"], token))
			{					
				if(token.size() == RESET_TOKEN_SIZE*2/*token size is specifed in bytes, string length is twice as long*/)
				{
					Database::obj db = get_database(request, "fenix");
					
					User::obj user = User::get(db);
					
					if(user->find(Query().add_cond("token", token)))
					{
						if(request.isRead())
						{
								return render_<ResetPassword::Page>((_token = token, _request = request, 
									_time = datetime::format(request._timestamp, "%H:%M %d-%b-%Y GMT+0")));			
						}
						
						if(request.isWrite() &&
							get_param(request["password"], password) &&
							get_param(request["repeat_password"], repeat_password))
						{
							if(password == repeat_password)
							{
								user->token("");
								user->set_password(password);
								user->save();
								
								request.setSession("user_id", user->id(), get_domain(request), 24*15);
								
								return render_redirect("/analytics/index");
							}
							else
							{
								return render_<ResetPassword::Page>((_token = token, _err_msg = 1, 
									_request = request, _time = datetime::format(request._timestamp, "%H:%M %d-%b-%Y GMT+0")));
							}
						}
					}
					else
					{
						return render_<ResetPassword::Page>((_token = "", _err_msg = 2, 
							_request = request, _time = datetime::format(request._timestamp, "%H:%M %d-%b-%Y GMT+0")));
					}
				}
			}
			
			return render_<BadRequestResponse>();
		}
	}
}
