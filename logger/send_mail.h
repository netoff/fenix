#pragma once

#include "lua.h"

namespace fenix
{
	namespace misc
	{
		namespace server
		{
			static const sendmail_cmd = "/usr/bin/sendmail";
			
			class system_mailer:private noncopyable
			{
			public:	
				system_mailer(const string& sender, const string& address)
				:_name(sender),_address(address)
				{					
				}
				~system_mailer()
				{
					if(this->_sendmail)
					{
						pclose(this->_sendmail);
					}
				}
				void send_mail(const string& to, const string subject, const string& body)
				{
					#ifdef DEBUG
					Log().get() << "Sending mail To: " << to << " Subject: " << subject
						<< " Body: " << body << " From: " << this->_name << ", " << this->_address;
						
					#else
					this->_open(to);
					
					if(this->_sendmail)
					{
						fprintf(this->_sendmail, "To: <%s>\r\n", to.c_str());
						fprintf(this->_sendmail, "From: %s<%s>\r\n", 
							this->_name.c_str(), this->_address.c_str());
						fprintf(this->_sendmail, "Subject: %s\r\n\r\n", subject.c_str());
						
						fprintf(this->_sendmail, "%s\r\n", body.c_str());
						
						this->_close();
					}
					#endif
				}
			private:
				string _name;
				string _address;
				
				FILE* _sendmail;
				
				void _open(const string& to)
				{
					this->_close();
					
					string sendmail = this->_sendmail_cmd(to);
					this->_sendmail = popen(sendmail.c_str(), "w");
				}
				void _close()
				{
					if(this->_sendmail)
					{
						pclose(this->_sendmail);
					}
				}
				
				string _sendmail_cmd(const string& to) const
				{
					return sendmail_cmd + " -F " + this->_name + " -f " + this->_address 
						+ " " + to;
				}				
			};
			
			static int _lua_send_mail(lua_State* _L)
			{
				string sender, from, to, subject, body; 
				
				if(lua_gettop(_L) > 4 && lua_isstring(_L, 1))
				{
					sender = string(lua_tostring(_L, 1));
					
					if(lua_isstring(_L, 2))
					{
						from = string(lua_tostring(_L, 2));
						if(lua_isstring(_L, 3))
						{
							to = string(lua_tostring(_L, 3));
							if(lua_isstring(_L, 4))
							{
								subject = string(lua_tostring(_L, 4));
								if(lua_isstring(_L, 5))
								{
									body = string(lua_tostring(_L, 5));
									system_mailer mailer(sender, from);
									
									mailer.send_mail(to, subject, body);
								}
							}
						}
					}
				}
				
				return 0;
			}
		}
	}
}
