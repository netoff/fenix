#pragma once

#include "common.h"

extern "C"
{
#include <GeoIP.h>
#include <GeoIPCity.h>
};

namespace fenix
{
	namespace web
	{
		namespace toolkit
		{
			namespace geo
			{
				class Location:private noncopyable
				{
				private:
					class IPRecord: private noncopyable
					{
					public:
						IPRecord(const string& ip, GeoIP* geoIP)
						:_gIPrec(0)
						{
							if(geoIP)
							{
								this->_gIPrec =  GeoIP_record_by_addr(geoIP, ip.c_str());
							}
						}
						
						~IPRecord()
						{
							if(this->_gIPrec)
							{
								GeoIPRecord_delete(this->_gIPrec);
							}
						}
						
						string country_code()const{return this->_gIPrec? _na(this->_gIPrec->country_code) : "";}
						//string continent()const{return _na()}
						string region()const{return this->_gIPrec? _na(this->_gIPrec->region) : "";}
						string city()const{return this->_gIPrec? _na(this->_gIPrec->city) : "";}
						string postal_code()const{return this->_gIPrec? _na(this->_gIPrec->postal_code) : "";}
						double longitude()const{return this->_gIPrec? this->_gIPrec->longitude : 0.0;}
						double latitude()const{return this->_gIPrec? this->_gIPrec->latitude : 0.0;}					
					private:
						 GeoIPRecord* _gIPrec;
						 
						 inline string _na(const char* s)
						 {
							 return s? string(s) : string("");
						 }
					};
				public:
					Location(const string& geo_ip_file)
					:_gIP(0)
					{
						this->_gIP = GeoIP_open(geo_ip_file.c_str(), GEOIP_MEMORY_CACHE);
					}
					
					~Location()
					{
						if(this->_gIP)
						{
							GeoIP_delete(this->_gIP);
						}
					}
					
					template <class Obj>
					void lookup(Obj& o, const string& ip)
					{
						if(this->_gIP)
						{
							IPRecord rec(ip, this->_gIP);
							
							o.country_code = rec.country_code();
							//o.continent = rec.continent();
							o.region = rec.region();
							o.city = rec.city();
							o.postal_code = rec.postal_code();
							
							o.longitude = rec.longitude();
							o.latitude = rec.latitude();
						}
					}					
				private:
					GeoIP* _gIP;
				};
			}
		}
	}
}
