#pragma once

extern const char* ROOT_PATH;

#include "fenix.h"

#include "controllers/dashboard_controller.h"
#include "controllers/stats_controller.h"

#include "controllers/login_controller.h"

#ifdef DEBUG
#include "controllers/debug_controller.h"
#endif

#include "controllers/log_controller.h"

#include "controllers/visitors_controller.h"

#include "controllers/pages_controller.h"
#include "controllers/referrers_controller.h"
#include "controllers/poll/visitors_controller.h"
#include "controllers/poll/pages_controller.h"
#include "controllers/poll/referrers_controller.h"
#include "controllers/poll/account_controller.h"

#include "controllers/admin_controller.h"

