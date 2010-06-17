#pragma once

#include <fenix.h>

using namespace fenix::web::toolkit;

struct LogRequest: private noncopyable
{
	string site_id;
	string event;
	
	string url;
	string title;
	
	string referrer;
	string orig_referrer;
	
	string agent;
	
	string country_code;
	string city;
	double longitude;
	double latitude;
	
	//string search_query;
	
	LogRequest(const string& id, const string& e)
	:site_id(id),event(e)
	{
		url = "";
		title = "";
		
		referrer = "";
		orig_referrer = "";
		
		agent = "";
		
		country_code = "";
		city = "";
		longitude = 0;
		latitude = 0;
	}	
	
	
	void load(const Request& request)
	{
		get_param(request["_u"], this->url);
		get_param(request["_t"], this->title);
		
		get_param(request["_r"], this->referrer);
		get_param(request["_o"], this->orig_referrer );
		
		//load from mod_geoip
		#ifdef DEBUG
		get_param(request["_c"], this->country_code);
		get_param(request["_cty"], this->city);	
		get_param(request["_lg"], this->longitude);
		get_param(request["_lt"], this->latitude);
		#else
		if(request["_demo"] == "J8c4USez")
		{
			get_param(request["_c"], this->country_code);
			get_param(request["_cty"], this->city);	
			get_param(request["_lg"], this->longitude);
			get_param(request["_lt"], this->latitude);
		}
		else
		{
			get_param(request.getParam("config", "GEOIP_COUNTRY_CODE"), this->country_code);
			get_param(request.getParam("config", "GEOIP_CITY"), this->city);		
			get_param(request.getParam("config", "GEOIP_LONGITUDE"), this->longitude);
			get_param(request.getParam("config", "GEOIP_LATITUDE"), this->latitude);
		}
		#endif
	}
	
	string to_object()
	{
		ostringstream ret;
		
		ret << "{"
			<< "id = " << escape(site_id, 32)//how many?
			<< ", url = " << escape(url, 1024) //1024
			<< ", title = " << escape(title, 128)//128
			
			<< ", referrer = " << escape(referrer, 1024)//1024
			<< ", orig_referrer = " << escape(referrer, 1024)
			<< ", country = " << escape(country_code, 3)
			<< ", city = " << escape(city, 64)
			<< ", longitude = " << longitude
			<< ", latitude = " << latitude
			<< "}";
		
		return ret.str();		
	}
private:
	
	string escape(const string& s, const int size=0)
	{
		string a = erase_all_copy(erase_all_copy(trim_copy(s), "[["), "]]");
		
		if(size && a.size() > size)
		{
			a = a.substr(0, size);
		}
		
		return "[[" + a + "]]";
	}
};

inline string ts_object(ptime ts)
{
	ostringstream ret;
	
	long long ts_micro = datetime::timestamp_micro(ts);
	long ts_s = datetime::timestamp(ts);
	long ts_m = datetime::timestamp_m(ts);
	long ts_h = datetime::timestamp_h(ts);
	long ts_d = datetime::timestamp_d(ts);
	
	ret << "{" 
		<< "ts_micro = " << ts_micro
		<< ", ts_s = " << ts_s 
		<< ", ts_m = " << ts_m 
		<< ", ts_h = " << ts_h 
		<< ", ts_d = " << ts_d << "}";
	
	return ret.str();
}
