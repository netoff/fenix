#include "controllers.h"
#include "aaa.h"

using namespace fenix::web::toolkit;

FENIX_APPLICATION(HelloWorld)
{
	typedef session_authentication::authenticate auth;
	
	auto_ptr<action::Base> action = dispatch(
			_("app")/"index"		<< _action<DashboardController::index, auth>() ||
			_("app")/"index"/"poll"		<< _action<StatsController::views, auth>() ||
			_("app")/"stats"/"views"	<< _action<StatsController::views, auth>() ||
			_("lg")/"view"			<< _action<LogController::lg>() ||
			_("app")/"visitors"		<< _action<PagesController::visitors, auth>() ||
			_("app")/"visitors"/"poll"	<< _action<PagesController::Poll::visitors, auth>() ||
			
			//Login and sigunp
			_("app")/"login"		<< _action<LoginController::index>() ||
			_("app")/"login"/"new"		<< _action<LoginController::account::create>() ||
			_("app")/"signup_thanks"	<< _action<LoginController::account::create_finish>() ||
			
			_("app")/"session"/"new"	<< _action<LoginController::session::create>() ||
			_("app")/"logout"		<< _action<LoginController::session::clear, auth>() ||
			
						
			_("app")/"pages"		<< _action<PagesController::pages, auth>()||
			_("app")/"referrers"		<< _action<PagesController::referrers, auth>()
			/*
			_("lg")/"hit"			<< _action<LogController::hit>(),
			_("lg")/"click"			<< _action<LogController::click>(),
			_("lg")/"event"			<< _action<LogController::event>()
			*/, request);

	return action->handle(request);
}
