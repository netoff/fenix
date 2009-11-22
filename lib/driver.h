/*
 * driver.h
 *
 *  Created on: Nov 10, 2009
 *      Author: duca
 *
 *      Simple protocol application driver
 */
#pragma once

#include "action.h"

using namespace fenix::web::toolkit;

extern "C" view::Response* handle(const action::Request& request);
