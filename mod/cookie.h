#pragma once

#include "common.h"
#include "action.h"
#include "datetime.h"
#include "crypto.h"

using namespace fenix::web::toolkit;

static const string expires_format = "%a, %d-%b-%Y %H:%M:%S GMT";

inline string _cookie_timestamp(int hrs)
{
	if(hrs == 0) { return ""; }
	
	ptime timestamp = second_clock::local_time();
	
	if(hrs < 0)
	{
		timestamp -= hours(24);//yesterday
	}
	if(hrs > 0)
	{
		timestamp += hours(hrs);
	}
	
	return datetime::format(timestamp, expires_format);
}
inline string _cookie_val(const string& val)
{
	/*string c = "";
	
	
	if(!val.empty())
	{
		c = val + crypto::hmac(val, fenix_secret_key);
	}
	
	return encode_param(c);*/
	return val;
}

/*
inline bool _cookie_val(const string& payload, string& val)
{
	string mac = "";
	string msg = "";
	
	if(payload.length() > crypto::hmac_size())
	{
		int offset = crypto::hmac_size();
		
		//mac = payload.substr(0, offset);		
		//msg = payload.substr(offset, payload.length() - offset);
		
		mac = payload.substr(payload.length() - offset, offset);
		msg = payload.substr(0, payload.length() - offset);
		
		
		if(crypto::hmac_verify(mac, msg, fenix_secret_key))
		{
			val = msg;
			
			return true;
		}
	}
	
	
	return false;
}*/

inline string cookie(const string& name, const string& val)
{	
	return str(format("%s=%s")%name %_cookie_val(val));
}

inline string cookie(const string& name, const string& val, const string& path)
{
	return str(format("%s=%s; path=%s")%name %_cookie_val(val) %path);
}

inline string cookie(const string& name, const string& val, const string& path, int expire)
{
	if(expire != 0)
	{
		string timestamp = _cookie_timestamp(expire);
	
		return str(format("%s=%s; expires=%s; path=%s")%name %_cookie_val(val) %timestamp %path);
	}
	else
	{
		return cookie(name, val, path);	
	}
}

inline string cookie(const string& name, const string& val, const string& path, const string& domain)
{
	return str(format("%s=%s; path=%s; domain=%s")%name %_cookie_val(val) %path %domain);
}

inline string cookie(const string& name, const string& val, const string& path, const string& domain, int expire)
{
	if(expire != 0)
	{
		string timestamp = _cookie_timestamp(expire);
	
		return str(format("%s=%s; expires=%s; path=%s; domain=%s;")%name %_cookie_val(val) %timestamp %path %domain);
	}
	else
	{
		return cookie(name, val, path, domain);
	}
}

inline void parse_cookies(const string& cookie, const action::Request& request)
{
	
	typedef char_separator<char> ch_sep;
	typedef tokenizer<ch_sep> tokenizer;

	ch_sep cookie_sep(";");
	ch_sep keyval_sep("=");

	tokenizer cookies(cookie, cookie_sep);

	foreach(string c, cookies)
	{
		trim(c);
		
		tokenizer kv(c, keyval_sep);
		vector<string> kv_vec(kv.begin(), kv.end());

		string key = (kv_vec.size() > 0)?decode_param(trim_copy(kv_vec[0])):"";
		string val = (kv_vec.size() > 1)?decode_param(trim_copy(kv_vec[1])):"";
		
		string cookie_val = "";
		
		//request.setSessionRaw(key, val);
		request._pparams[key] = action::cookie_param(val, -1);
		/*
		if(_cookie_val(val, cookie_val))
		{
			//incoming cookies are marked with -1
			session[key] = action::cookie_param(cookie_val, -1);
		}*/
	}
}

