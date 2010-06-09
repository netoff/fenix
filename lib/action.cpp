#include "action.h"

extern const char* app_secret_key;

namespace fenix
{
	namespace web
	{
		namespace toolkit
		{
			namespace action
			{				
				bool _get_val(const string& raw, string& val)
				{
					val = "";
					
					if(raw.length() > crypto::hmac_size())
					{
						string mac = "";
						string msg = "";
						
						int offset = crypto::hmac_size();
						
						mac = raw.substr(raw.length() - offset, offset);
						msg = raw.substr(0, raw.length() - offset);
												
						if(crypto::hmac_verify(mac, msg, app_secret_key))
						{
							val = msg;							
							return true;
						}
					}					
					
					return false;
				}
				
				string _get_val(const string& val)
				{
					return val + crypto::hmac(val, app_secret_key);
				}
			}
		}
	}
}
