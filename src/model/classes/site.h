#pragma once

#include "user.h"
#include "../db_conf.h"

#include "fenix.h"

//now region is fixed should become flexible
static const string region_one = "a1";
static const int key_offset = 10101;

namespace tables
{
	class Site: public Table<Site>
	{
	public:
		TABLE(Site)
		{
		}
		
		STRING_FIELD(url)
		STRING_FIELD(timezone)
		STRING_FIELD(key)
		
		
		FOREIGN_KEY_FIELD(user)
	protected:
		/*
		virtual void _on_create(const string& id)
		{
			int64_t guid = 0;
			
			if(get_param(id, guid))
			{
				string key = id_to_key(guid, region_one);
				
				this->key(key);
			}
		}
		
		int64_t key_to_id(const string& key, string& region)
		{
			if(key.length() > 1)
			{
				region = key[0] + key[1];
			}
			
			string id(key.begin() + 2, key.end());
			
			return crypto::base36(id) - key_offset;
		}
		
		string id_to_key(int64_t id, const string& region)
		{
			id += key_offset;
			
			string base36 = crypto::base36(id);
			
			while(base36.length() < 6)
			{
				base36 = "0" + base36;
			}
			
			return region + base36;
		}*/
	};
}
