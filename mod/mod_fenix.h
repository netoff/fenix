#pragma once

#include <httpd.h>
#include <http_request.h>
#include <http_core.h>
#include <http_config.h>
#include <http_protocol.h>

#include <apr_strings.h>
#include <apr_tables.h>

 extern "C" __attribute__((visibility("default"))) module AP_MODULE_DECLARE_DATA fenix_module;
