#pragma once

#include "common.h"
#include "html_tags.h"

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
				class Response
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
						AJAX
					};
					enum ResponseVersion
					{
						v1_0
					};

					typedef ResponseType Type;
					typedef ResponseVersion Version;

					Type getResponseType(){return _type;}

					string getResponseBody(){return _response;}
					const char* getResponseBodyC(){return _response.c_str();}

					void setMimeType(string mime_type){_mime_type = mime_type;}
					string getMimeType(){return _mime_type;}
					const char* getMimeTypeC(){return _mime_type.c_str();}

					virtual ~Response(){}
				protected:
					Response(ResponseType t, int reserve=0, string mime_type="text/html")
						:_type(t)
					{
						if(reserve != 0)
							_response.reserve(reserve);
						_mime_type = mime_type;
					}

					Type _type;

					string _response;
					string _mime_type;
				};

				class InlineResponse: public Response
				{
				public:
					InlineResponse(string& body, string mime_type)
						:Response(Response::PLAIN_TEXT, 4*1024, mime_type)
					{
						_response.append(body);
					}
				};

				class FileResponse: public Response
				{
				public:
					static const char* mime_types[23][2];

					typedef const char** s_array;
					static string get_file_mime_type(string filename)
					{
						foreach(s_array s, mime_types)
						{
							if(ends_with(filename, s[0]))
								return s[1];
						}
						return "";
				
					}

					FileResponse(string filename, string home)
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
				public:
					FileNotFoundResponse()
						:Response(Response::NOT_FOUND, 0, "text/html")
					{
						_response = "File not found!";
					}
				};

				class ForbidenResponse: public Response
				{
				public:
					ForbidenResponse()
						:Response(Response::FORBIDEN, 0, "text/html")
					{
						_response = "Action forbiden!";
					}
				};

				class BadRequestResponse: public Response
				{
				public:
					BadRequestResponse()
						:Response(Response::BAD_REQUEST, 0, "text/html")
					{
						_response = "Invalid request!";
					}
				};

				class HTMLResponse: public Response
				{
				public:
					HTMLResponse(string html)
						:Response(Response::HTML)
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
					HTMLResponse(ResponseType response_type, string mime_type)
						:Response(response_type, 32*1024, mime_type){}
				};

				class DHTMLResponse: public HTMLResponse
				{
				public:
					DHTMLResponse()
						:HTMLResponse(Response::HTML){}
				protected:
					void insert(string a, bool new_line=true);

					template <class T>
					void insert(string a, const T& param, bool new_line=true)
					{
						insert(str(format(a)%param), new_line);
					}

					template <class T1, class T2>
					void insert(string a, T1& param1, T2& param2, bool new_line=true)
					{
						insert(str(format(a)%param1%param2));
					}
					
					template <class T1, class T2, class T3>
					void insert(string a, T1& param1, T2& param2, T3& param3, bool new_line=true)
				    {
							insert(str(format(a)%param1%param2%param3));
					}

					template <class T1, class T2, class T3, class T4>
					void insert(string a, T1& param1, T2& param2, T3& param3, T4& param4, bool new_line=true)
				    {
							insert(str(format(a)%param1%param2%param3%param4));
					}
					
					template <class T>
					void echo(const T& a){insert(escape(_to_s(a)), false);}
					void print(string a){insert(sanatize(a));}

					void include_js(string js_file)
					{
						_js_includes.push_back(js_file);
					}

					string _title;

				protected:
					vector<string> _js_includes;

				private:
					string escape(string s);
					string sanatize(string s);

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

			}
		}
	}
}
