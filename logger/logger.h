#pragma once

#include <boost/asio.hpp>
#include <boost/threadpool.hpp>
//#include <boost/thread.hpp>
#include <boost/bind.hpp>
//#include <boost/date_time/posix_time/posix_time.hpp>
#include <datetime.h>


#include "request.h"

using namespace boost;
using namespace fenix::web::toolkit;

static const string local_endpoint = "/tmp/fenix";
static const int num_th = 64;

static const posix_time::time_duration min_interval = posix_time::minutes(1);
static const posix_time::time_duration hour_interval = posix_time::hours(1);
static const posix_time::time_duration day_interval = posix_time::hours(12);

static string cron_minutely(const string& ts)
{
	return "cron._cron_minutely(" + ts + ")";
}
static string cron_hourly(const string& ts)
{
	return "cron._cron_hourly(" + ts + ")";
}
static string cron_daily(const string& ts)
{
	return "cron._cron_daily(" + ts + ")";
}

namespace fenix
{
	namespace misc
	{
		namespace server
		{		
			inline string ts_object(ptime ts)
			{
				ostringstream ret;
				
				long ts_s = datetime::timestamp(ts);
				long ts_m = datetime::timestamp_m(ts);
				long ts_h = datetime::timestamp_h(ts);
				long ts_d = datetime::timestamp_d(ts);
				
				ret << "{" 
					<< "ts_s = " << ts_s
					<< ", ts_m = " << ts_m 
					<< ", ts_h = " << ts_h 
					<< ", ts_d = " << ts_d << "}";
				
				return ret.str();
			}
			
			typedef asio::local::stream_protocol::acceptor ACCEPTOR_TYPE;
			typedef threadpool::pool THREADPOOL_TYPE;
			typedef asio::io_service IO_SERVICE_TYPE;
			typedef asio::deadline_timer TIMER_TYPE;
			
			class logger_server: private noncopyable
			{
			public:
				logger_server()
				:_ios(),_acceptor(_ios, asio::local::stream_protocol::endpoint(local_endpoint)),
				_timer_min(_ios),_timer_hour(_ios),_timer_day(_ios)
				,_request(logger_request::get(_ios))
				{
					this->_acceptor.listen();
					
					this->_loop();
					
					this->_loop_timer_min();
					this->_loop_timer_hour();
					this->_loop_timer_day();
				}
				
				void run()
				{
					THREADPOOL_TYPE pool(num_th);
					
					for(int i = 0; i < num_th; i++)
					{
						pool.schedule(bind(&IO_SERVICE_TYPE::run, &this->_ios));
					}					
				}
				
				void stop()
				{
					this->_ios.stop();
				}
			private:
				//asio::ip::tcp::acceptor _acceptor;
				IO_SERVICE_TYPE _ios;
				ACCEPTOR_TYPE _acceptor;
				
				TIMER_TYPE _timer_min;
				TIMER_TYPE _timer_hour;
				TIMER_TYPE _timer_day;				
				
				logger_request::obj _request;				
				
				void _loop()
				{
					this->_acceptor.async_accept(this->_request->socket(),
						bind(&logger_server::_handle_accept, this,
							asio::placeholders::error));					
				}
				
				void _loop_timer_min()
				{
					this->_timer_min.expires_from_now(min_interval);
					this->_timer_min.async_wait(
						bind(&logger_server::_handle_timer_min, this, 
							asio::placeholders::error));
				}
				void _loop_timer_hour()
				{
					this->_timer_hour.expires_from_now(hour_interval);
					this->_timer_hour.async_wait(
						bind(&logger_server::_handle_timer_hour, this,
							asio::placeholders::error));
				}
				void _loop_timer_day()
				{
					this->_timer_day.expires_from_now(day_interval);
					this->_timer_day.async_wait(
						bind(&logger_server::_handle_timer_day, this,
							asio::placeholders::error));
				}
				
				void _handle_accept(const system::error_code& error)
				{
					if(!error)
					{
						this->_request->run();
						this->_request = logger_request::get(this->_ios);						
					}
					this->_loop();
				}
				
				void _handle_timer_min(const boost::system::error_code& error)
				{
					if(!error)
					{
						ScopedEngine en;
						
						ptime timestamp = microsec_clock::universal_time();					
						
						en->proc(cron_minutely(ts_object(timestamp)));
						
						en.done();
					}
					
					this->_loop_timer_min();
				}
				
				void _handle_timer_hour(const boost::system::error_code& error)
				{
					if(!error)
					{
						ScopedEngine en;
						ptime timestamp = microsec_clock::universal_time();
						
						en->proc(cron_hourly(ts_object(timestamp)));
						en.done();
					}
					
					this->_loop_timer_hour();
				}
				void _handle_timer_day(const boost::system::error_code& error)
				{
					if(!error)
					{
						ScopedEngine en;
						ptime timestamp = microsec_clock::universal_time();
						
						en->proc(cron_daily(ts_object(timestamp)));
						en.done();
					}
					
					this->_loop_timer_day();
				}
			};
		}
	}
}
