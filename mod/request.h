#pragma once

#include <driver.h>

#include <httpd.h>

using namespace std;
using namespace fenix::web::toolkit::action;

apr_status_t send_apc_file(string filename, string mime_type, request_rec* apc_request);
void prepare_request(Request& request, request_rec* apc_request);
