#pragma once

#include "common.h"
#include "html_tags.h"
#include "log.h"

#include "boost/filesystem.hpp"
#include "boost/lexical_cast.hpp"
#include "boost/algorithm/string.hpp"

using namespace boost;
using namespace boost::filesystem;

namespace fenix
{
	namespace web
	{
		namespace toolkit
		{
			namespace view
			{
				static const string pixel_bin = "R0lGODlhAQABAIAAAAAAAAAAACH5BAEAAAAALAAAAAABAAEAAAICRAEAOw==";
				
				class Response: private noncopyable
				{
				public:
					enum ResponseType
					{
						HTML,
						DHTML,
						REDIRECT,
						FILE,
						INTERNAL_ERROR,
						
						UNSUPPORTED,
						RESTRICT,
						NOT_FOUND,
						FORBIDEN,
						BAD_REQUEST,

						XML,
						PLAIN_TEXT,
						NEXT_ACTION,
						MESSAGE,
						JSON,
						JAVASCRIPT,
						EXCEPTION,
						AJAX,
						//base_64 encoded, eg. tracking pixel
						BINARY
					};
					enum ResponseVersion
					{
						v1_0
					};

					typedef ResponseType Type;
					typedef ResponseVersion Version;

					Type getResponseType() const { return _type; }

					string getResponseBody() const { return _response; }
					const char* getResponseBodyC() const { return _response.c_str(); }

					void setMimeType(string mime_type) { _mime_type = mime_type; }
					
					string getMimeType() const { return _mime_type; }
					const char* getMimeTypeC() const { return _mime_type.c_str(); }
					
					//string get_log_output() const { return log::_flush_log(); }

					virtual ~Response(){}
				protected:
					Response(ResponseType t, int reserve, const string& mime_type)
					:_type(t),_mime_type(mime_type),_response()
					{
						if(reserve > 0)
						{
							_response.reserve(reserve);
						}
						
						_mime_type = mime_type; 
					}

					Type _type;
					string _mime_type;
					
					string _response;
				};

				class InlineResponse: public Response
				{
					static const int _response_size = 4*1024;
				public:
					InlineResponse(const string& body, const string& mime_type)
						:Response(Response::PLAIN_TEXT, _response_size, mime_type)
					{
						_response.append(body);
					}
				};
				
				class RedirectResponse: public Response
				{
					static const int _response_size = 1024;
				public:
					RedirectResponse(const string& url)
					:Response(Response::REDIRECT, _response_size, "text/plain")
					{
						_response.append(url);
					}
				};

				class FileResponse: public Response
				{
				public:
					static const char* mime_types[23][2];

					typedef const char** s_array;
					static string get_file_mime_type(const string& filename)
					{
						foreach(s_array s, mime_types)
						{
							if(ends_with(filename, s[0]))
								return s[1];
						}
						
						return "";				
					}

					FileResponse(const string& filename, const string& home)
						:Response(Response::FILE, 0, get_file_mime_type(filename))
					{
						path p = _get_file_path(filename, home);

						_response = p.string();
					}

					static bool getFileResponse(string filename, string home, FileResponse** file_response)
					{
						path p = _get_file_path(filename, home);

						if(exists(p) && !is_directory(p))
						{
							*file_response = new FileResponse(filename, home);
							return true;
						}
						else
						{
							file_response = 0;
							return false;
						}

					}
				private:
					static path _get_file_path(string filename, string home)
					{
						return complete(path(filename), path(home));
					}
				};

				class FileNotFoundResponse: public Response
				{
					static const int _response_size = 0;
				public:
					FileNotFoundResponse()
						:Response(Response::NOT_FOUND, _response_size, "text/html")
					{
						_response = "File not found!";
					}
				};

				class ForbidenResponse: public Response
				{
					static const int _response_size = 0;
				public:
					ForbidenResponse()
						:Response(Response::FORBIDEN, _response_size, "text/html")
					{
						_response = "Action forbiden!";
					}
				};

				class BadRequestResponse: public Response
				{
					static const int _response_size = 0;
				public:
					BadRequestResponse()
						:Response(Response::BAD_REQUEST, _response_size, "text/html")
					{
						_response = "Invalid request!";
					}
				};
				
				class NotFoundResponse: public Response
				{
					static const int _response_size = 0;
				public:
					NotFoundResponse(const string& path="")
						:Response(Response::NOT_FOUND, _response_size, "text/html")
					{
						ostringstream out;
						
						out << "<html>";
						out << "<head><title>Dispatcher exception</title></head>";
						out << "<body>";
						out << "<h1>Dispatcher exception</h1>";
						
						if(!path.empty())
						{
							out << "<p>" << path << " can not be found!";
						}
						else
						{
							
							out << "<p>Page can not be found!</p>";
						}
						out << "</body></html>";
						
						_response = out.str();
					}
				};
				
				class TrackingPixel: public Response
				{				
				public:
					TrackingPixel()
					:Response(Response::BINARY, 64, "image/gif")
					{
						this->_response = pixel_bin;
					}
				};

				class HTMLResponse: public Response, private noncopyable
				{
				public:
					HTMLResponse(string html)
						:Response(Response::HTML, html.size(), "text/html")
					{
						_response = html;
					}

					string getHTML(){return _response;}

					void persist_to_disk(string filename, string home)
					{
						path p = path(home)/path(filename);
						ofstream output_file(p.file_string().c_str());

						output_file << _response;

						output_file.close();
					}

					virtual ~HTMLResponse(){};
				protected:
					HTMLResponse(ResponseType response_type)
						:Response(response_type, 32*1024, "text/html"){}
					HTMLResponse(ResponseType response_type, const string& mime_type)
						:Response(response_type, 32*1024, mime_type){}
				};

				class DHTMLResponse: public HTMLResponse, private noncopyable
				{
				public:
					DHTMLResponse()
						:HTMLResponse(Response::HTML){}
						
					virtual ~DHTMLResponse(){}
				protected:
					void insert(const string& a, bool new_line=true);

					template <class T>
					void insert(const string& a, const T& param, bool new_line=true)
					{
						insert(str(format(a)%param), new_line);
					}

					template <class T1, class T2>
					void insert(const string& a, const T1& param1, const T2& param2, bool new_line=true)
					{
						insert(str(format(a)%param1%param2));
					}
					
					template <class T1, class T2, class T3>
					void insert(const string& a, const T1& param1, const T2& param2, const T3& param3, bool new_line=true)
					{
						insert(str(format(a)%param1%param2%param3));
					}

					template <class T1, class T2, class T3, class T4>
					void insert(const string& a, const T1& param1, const T2& param2, const T3& param3, const T4& param4, bool new_line=true)
					{
						insert(str(format(a)%param1%param2%param3%param4));
					}
					
					template <class T>
					void echo(const T& a) { insert(escape(_to_s(a)), false); }
					
					void print(const string& a) { insert(sanatize(a)); }

					void include_js(const string& js_file)
					{
						_js_includes.push_back(js_file);
					}

					void include_css(const string& css_file)
					{
						_css_includes.push_back(css_file);
					}
					
					string _title;
				protected:
					vector<string> _js_includes;
					vector<string> _css_includes;
					
					string escape(const string& s);
					string sanatize(const string& s);
				private:
					template <class T>
					string _to_s(T a)
					{
						string ret = "";
						try
						{
							ret = lexical_cast<string>(a);
						}
						catch(bad_lexical_cast&)
						{
							ret = "";
						}
						return ret;
					}

					string _to_s(bool a)
					{
						if(a)
							return "true";
						else
							return "false";
					}
				};
				
				struct _deleter
				{
					template<class T>
					void operator() (T* t)
					{
						delete t;
					}
				};
				
				template <class T>
				shared_ptr<T> render_()
				{
					shared_ptr<T> p(new T(), _deleter());
					
					return p;
				}
				
				template <class T, class ArgumentsPack>
				shared_ptr<T> render_(const ArgumentsPack& args)
				{
					shared_ptr<T> p(new T(args), _deleter());
					
					return p;
				}
				
				inline shared_ptr<InlineResponse> render_text(const string& body, const string& mime_type)
				{
					shared_ptr<InlineResponse> p(new InlineResponse(body, mime_type), _deleter());
					
					return p;
				}
				
				inline shared_ptr<InlineResponse> render_text(const ostringstream& body, const string& mime_type)
				{
					return render_text(body.str(), mime_type);
				}
				
				inline shared_ptr<RedirectResponse> render_redirect(const string& url)
				{
					shared_ptr<RedirectResponse> p(new RedirectResponse(url), _deleter());
					
					return p;
				}
				
				inline shared_ptr<RedirectResponse> render_redirect(const string& root, const string& url)
				{
					shared_ptr<RedirectResponse> p(new RedirectResponse(root + url), _deleter());
					
					return p;
				}
			}
		}
	}
}
