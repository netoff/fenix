#include "middleware.h"
#include "log.h"

namespace fenix
{
	namespace web
	{
		namespace toolkit
		{
			MiddlewarePool mid_pool;
			
			string Middleware::_connect(const string& header, const string& msg)
			{
				boost::system::error_code error;
					
				asio::streambuf _request;
				ostream request(&_request);
				
				request << header << "\r\n";
				
				if(!msg.empty())
				{
					request << msg << "\r\n\r\n";
				}
				
				asio::write(this->_socket, _request, transfer_all(), error);
				
				if(error) 
				{
					this->_bad = true;
					return "";//
				}
				
				asio::streambuf _response;
				asio::read_until(this->_socket, _response, "\r\n", error);
				
				if(error)
				{
					this->_bad = true;
					return "";
				} 
				
				istream response(&_response);
				
				string status;
				getline(response, status);
				
				trim_if(status, is_space());
				if(status != "ok")
				{				
					return "";
				}
				
				string ret = "";
				//read any exces left from previous read_until call
				
				string l;
				bool first = true;/*
				while(getline(response, l))
				{
					trim_if(l, is_space());
					if(first)
					{
						first = false;
					}
					else
					{
						ret.append("\n");
					}
					ret.append(l);
					
				}*/				
				//read up to the eof
				//while(true)
				//{
				asio::read_until(this->_socket, _response, "\r\n\r\n", error);
		
				response.clear();
				first = true;					
				while(getline(response, l))
				{
					trim_if(l, is_space());
					if(first)
					{
						first = false;
					}
					else
					{
						ret.append("\n");
					}
					ret.append(l);					
				}
				
				if(error)
				{
					this->_bad = true;
					return "";
				}
				//}
				/*
				if (error != asio::error::eof)
				{
					log::getLOG << "Middleware - Error getting server response!";
					
					return "";
				} */
				
				return ret;
			}
		}
	}
}
