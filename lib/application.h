#pragma once

#include "common.h"
#include "action.h"
#include "model.h"
#include "view.h"

#define FENIX_APPLICATION(app)								\
	extern "C" __attribute__((visibility("default"))) 				\
			shared_ptr<view::Response> handle(const action::Request& request)\
