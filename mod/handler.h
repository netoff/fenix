#pragma once

#include <httpd.h>
#include <http_request.h>
#include <http_core.h>
#include <http_protocol.h>
#include <http_config.h>

int fenix_handler(request_rec* apc_request);
