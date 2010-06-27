#pragma once

#include "../db_conf.h"
#include "fenix.h"

#include "datetime.h"

using namespace fenix::web::toolkit;

namespace tables
{
	class User: public Table<User>
	{
	public:
		TABLE(User)
		{
			this->_password_set = false;
		}
		
		STRING_FIELD(email)
		STRING_FIELD(state)
		
		STRING_FIELD_A(plain_password)
		
		STRING_PRIV(password)
		STRING_PRIV(salt)
		
		HAS(sites, Site, user_id)
		
		STRING_FIELD(role)
		
		STRING_FIELD(timestamp)
		STRING_FIELD(registration_date)
		STRING_FIELD(last_login)
		STRING_FIELD(last_activity)
		
		STRING_FIELD(token)

		bool authenticate()
		{
			string digest = crypto::sha1(this->salt() + this->plain_password());
			string pwd = this->password();
			
			return (digest == pwd);
		}
		
		bool activated()
		{
			return (this->state() == "activated");
		}
		bool is_activated()
		{
			return this->activated();
		}
		
		void activate()
		{
			this->state("activated");
		}
		
		void log_login()
		{
			ptime now = microsec_clock::universal_time();
			
			this->last_login(datetime::format(now, "%H:%M %d-%b-%Y GMT+0"));
		}
		
		void log_activity()
		{
			ptime now = microsec_clock::universal_time();
			
			this->last_activity(datetime::format(now, "%H:%M %d-%b-%Y GMT+0"));
		}
		
		void set_password(const string& plain)
		{
			this->_password_set = true;
			this->plain_password(plain);			
		}
	protected:
		/*virtual bool _before_save()
		{
			if(!plain_password().empty() && plain_password().length() > 5)
			{
				this->password(crypto::sha1(this->plain_password(), this->salt()) );
			}
			
			return true;
		}*/
		
		virtual bool _before_create()
		{
			string slt = crypto::random<32>();
			string plain = this->plain_password();
			
			string pwd = crypto::sha1(slt + plain);
			
			this->password(pwd);
			this->salt(slt);
			
			this->role("admin");			
			this->state("registered");
			
			ptime now = microsec_clock::universal_time();
			
			this->registration_date(datetime::format(now, "%H:%M %d-%b-%Y GMT+0"));
			this->timestamp(to_string(datetime::timestamp(now)));
			
			return true;			
		}
		
		virtual bool _before_save()
		{
			if(this->_password_set)
			{
				if(!plain_password().empty() && plain_password().length() > 5)
				{
					this->password(crypto::sha1(this->salt() + this->plain_password()));
				}
			}
			
			return true;
		}
	private:
		bool _password_set;
	};
}
