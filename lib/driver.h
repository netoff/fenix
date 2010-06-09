#pragma once

#include "action.h"

using namespace fenix::web::toolkit;

extern "C" view::Response* handle(const action::Request& request);
