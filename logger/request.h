#pragma once

#include <boost/asio.hpp>
#include "common.h"

#include "engine_pool.h"
#include "log.h"

using namespace boost;

namespace fenix
{
	namespace misc
	{
		namespace server
		{
			typedef asio::local::stream_protocol::socket SOCKET_TYPE;
			typedef asio::streambuf BUFFER_TYPE;
			typedef asio::strand STRAND_TYPE;
			
			class logger_request: public enable_shared_from_this<logger_request>,
														private noncopyable
			{
			public:
				typedef shared_ptr<logger_request> obj;
				static obj get(asio::io_service& ios)
				{
					obj ret(new logger_request(ios));
						
					return ret;
				}
				
				SOCKET_TYPE& socket()
				{
					return this->_socket;
				}
				
				//enter the loop
				void run()
				{
					try
					{
						//string message = "Hello from server: \n";
						
						//asio::read_until(this->_socket, this->_buf, "\r\n");
						
						//read first line
						this->_start_request();
						
						//shared_from_this().reset();//OK???
					}
					catch(std::exception& e)
					{
					}
				}
				
				void handle_first_line(const boost::system::error_code& error)
				{	
					if(!error)
					{	
						string method = this->read_line();
						
						if(method != "query" && method != "insert" && method != "test" && method != "close")
						{
							this->_response = "error";
							this->_result = "unknown method";
							this->send_result();
						
							this->_start_request();
						
							return;
						}
						else
						{
							if(method == "query")
							{
								this->_handle_request_sync();
								return ;
							}
							if(method == "insert")
							{
								this->_handle_request_async();
								return;
							}
							if(method == "test")
							{
								this->_handle_request_test();
								return;
							}
							if(method == "close")
							{
								this->handle_close();
							}
						}
					}
					else
					{
						Log().get() << "Error: " << error.message();
						
						this->handle_close();
						return;
					}
				}
				
				void handle_request_async(const boost::system::error_code& error)
				{
					if(!error)
					{
						this->_query = this->read_buffer();						
						
						this->_finish_request_async();
					}
					else
					{
						this->handle_close();
					}
				}
				
				void finish_request_async(const boost::system::error_code& error)
				{
					if(!error)
					{
						ScopedEngine engine;
						
						if(!engine->proc(this->_query))
						{
							Log().get() << "Error while async exec. Code: " << this->_query;						
						}
						
						engine.done();
						
						this->_start_request();
					}
					else
					{
						this->handle_close();
					}
				}
				
				void handle_request_sync(const boost::system::error_code& error)
				{
					if(!error)
					{
						this->_query = this->read_buffer();
					
						ScopedEngine engine;
						
						if(engine->proc(this->_query, this->_result))
						{
							this->_response = "ok";
							this->send_result();
						}
						else
						{
							this->_response = "error";
							this->_result = "error trying to execute code";
							this->send_result();
						}
						
						engine.done();
					
						this->_start_request();
					}
					else
					{
						this->handle_close();
					}
				}
				
				void handle_request_test()
				{
					this->_response = "ok";
					this->_result = "hello from server";
					this->send_result();
					
					this->_start_request();
				}
				
				void handle_close()
				{
					boost::system::error_code ec;
					this->_socket.shutdown(asio::local::stream_protocol::socket::shutdown_both, ec);
					
					if(this->_socket.is_open())
					{
						this->_socket.close();
					}
					
					//delete this;
				}
				
				~logger_request()
				{
					//this->_engine.done();
				}
				
			private:
				SOCKET_TYPE _socket;
				BUFFER_TYPE _buf;
				STRAND_TYPE _strand;
				
				//ScopedEngine _engine;
				
				string _response;
				string _result;
				
				string _query;
				string _out;
				
				logger_request(asio::io_service& ios)
				:_socket(ios),_buf(),_strand(ios),
				_response(),_result()
				{
				}
				
				void _start_request()
				{
					//should we clear buf?? this way?
					if(this->_buf.size() > 0)
					{
						this->_buf.consume(this->_buf.size());
					}
					
					asio::async_read_until(this->_socket, this->_buf, "\r\n",
						this->_strand.wrap(bind(&logger_request::handle_first_line, shared_from_this(), 
							boost::asio::placeholders::error)));
				}
				
				void _handle_request_async()
				{
					asio::async_read_until(this->_socket, this->_buf, "\r\n\r\n", 
						this->_strand.wrap(bind(&logger_request::handle_request_async, shared_from_this(),
							boost::asio::placeholders::error)));
				}
				
				void _finish_request_async()
				{
					this->_out = "ok\r\nrequest accepted\r\n\r\n";
					
					asio::async_write(this->_socket, asio::buffer(this->_out), 
						this->_strand.wrap(bind(&logger_request::finish_request_async, shared_from_this(),
							boost::asio::placeholders::error)));
				}
				
				void _handle_request_sync()
				{
					asio::async_read_until(this->_socket, this->_buf, "\r\n\r\n", 
						this->_strand.wrap(bind(&logger_request::handle_request_sync, shared_from_this(),
							boost::asio::placeholders::error)));
				}
				
				void _handle_request_test()
				{
					handle_request_test();
				}
				
				string read_line()
				{
					istream is(&(this->_buf));
					string ret = "";
					
					getline(is, ret);
					
					trim_if(ret, is_space());
					
					return ret;
				}
				
				string read_buffer()
				{
					istream is(&(this->_buf));
					string ret = "";
					
					string l;
					bool first = true;
					while(getline(is, l))
					{
						trim_if(ret, is_space());
						
						if(first)
						{
							ret.append(l);
							first = false;
						}
						else
						{
							ret.append("\n" + l);
						}
					}
					
					return ret;				
				}
				
				void send_result()
				{
					this->_out = "";
					
					this->_out += this->_response + "\r\n";
					this->_out += this->_result + "\r\n\r\n";
					
					//asio::write(this->_socket, asio::buffer(this->_out), asio::transfer_all());
					asio::async_write(this->_socket, asio::buffer(this->_out), asio::transfer_all(),
						this->_strand.wrap(bind(&logger_request::_handle_write, shared_from_this(),
							asio::placeholders::error)));
						
					//this->_socket.close();
				}
				
				void _handle_write(const boost::system::error_code& error)
				{
					if(error)
					{
						this->handle_close();
					}
				}
				
			};
		}
	}
}
