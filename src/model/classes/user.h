#pragma once

#include "../db_conf.h"
#include "fenix.h"

using namespace fenix::web::toolkit;

namespace tables
{
	class User: public Table<User>
	{
	public:
		User(DB db)
		:Table<User>(db, "User")
		{
		}
		
		STR_FIELD(email)
		STR_FIELD(state)
		
		STR_FIELD_A(plain_password)
		
		STR_FIELD_PRIV(password)
		STR_FIELD_PRIV(salt)
		
		HAS(sites, Site, user_id)

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
			
			this->state("registered");
			
			return true;			
		}
	};
}
