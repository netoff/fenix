#pragma once

#include "common.h"
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/date_time/local_time/local_time.hpp>
#include <iostream>
#include <ctime>

#include "log.h"

using namespace boost::gregorian;
using namespace boost::local_time;
using namespace boost::posix_time;

namespace fenix
{
	namespace web
	{
		namespace toolkit
		{
			namespace datetime
			{
				static const ptime fenix_epoch(date(2001, 1, 1));
				
				inline long timestamp(const local_date_time& local_time)
				{
					time_duration diff = (local_time.utc_time() - fenix_epoch);
					return diff.total_seconds();
				}
				
				inline time_duration diff(const ptime& t)
				{
					return t - fenix_epoch;
				}
				
				inline date_duration diff_d(const ptime& t)
				{
					return t.date() - fenix_epoch.date();
				}

				inline long timestamp(const ptime& t)
				{
					return diff(t).total_seconds();
				}
				
				inline long timestamp_m(const ptime& t)
				{
					//TODO: check is this reliable and proper way
					//to calculate minutes count
					
					time_duration d = diff(t);
					
					long hours = d.hours();
					long minutes = d.minutes();
					
					//total minutes
					return hours * 60/*minutes per hour*/ + minutes;
				}
				
				inline long timestamp_h(const ptime& t)
				{
					return diff(t).hours();
				}
				
				inline long timestamp_d(const ptime& t)
				{
					return diff_d(t).days();
				}
				
				inline long timestamp_d(const local_date_time& l)
				{
					return (l.date() - fenix_epoch.date()).days();
				}

				inline ptime from_timestamp(const long& timestamp)
				{
					ptime t(fenix_epoch.date(), seconds(timestamp));
					return t;
				}
				
				inline string format(const ptime& t, const string& fmt)
				{	
					string ret = "";
					
					const int max_size = 256;
					
					char buf[max_size];
					
					tm c_time = to_tm(t);
					
					int length = strftime(buf, max_size, fmt.c_str(), &c_time);
					
					if(length > 0)
					{
						ret.assign(buf);
					}
					
					return ret;
				}
			}
		}
	}
}
