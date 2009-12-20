#pragma once

#include <boost/date_time/local_time/local_time.hpp>

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
				const ptime fenix_epoch(date(2001, 1, 1));

				inline long timestamp(const local_date_time& local_time)
				{
					time_duration diff = (local_time.utc_time() - fenix_epoch);
					return diff.total_seconds();
				}

				inline long timestamp(const ptime& utc_time)
				{
					time_duration diff = utc_time - fenix_epoch;
					return diff.total_seconds();
				}

				inline ptime from_timestamp(const long& timestamp)
				{
					ptime t(fenix_epoch.date(), seconds(timestamp));
					return t;
				}
			}
		}
	}
}
