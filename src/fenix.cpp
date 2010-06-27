#include "controllers.h"
#include "aaa.h"

extern const char* ROOT_PATH = "analytics";
extern const char* app_secret_key = SECRET_KEY;

using namespace fenix::web::toolkit;

template <int T>
struct auth
{
	typedef session_authentication::authenticate<T> Type;
};

FENIX_APPLICATION(HelloWorld)
{	
	auto_ptr<action::Base> action = dispatch(
		#ifdef DEBUG
			_root/"_debug"/"db" 		<< _action<DebugController::db>() ||
		#endif			
			_root/"index"		<< _action<DashboardController::index, auth<1>::Type>() ||			
			
			_("lg")/"js"			<< _action<LogController::lg>() ||
			
			//Login and sigunp
			_root/"login"		<< _action<LoginController::index>() ||
			_root/"login"/"new"		<< _action<LoginController::account::create>() ||
			_root/"login"/"forgot_password" << _action<LoginController::account::forgot>()||
			_root/"login"/"reset"		<< _action<LoginController::account::reset>() ||			
			_root/"session"/"new"	<< _action<LoginController::session::create>() ||
			_root/"logout"		<< _action<LoginController::session::clear, auth<1>::Type>() ||
			//=======================================================================			
			
			_root/"visitors"		<< _action<PagesController::visitors, auth<1>::Type>() ||					
			_root/"pages"		<< _action<PagesController::pages, auth<1>::Type>()||			
			_root/"referrers"		<< _action<PagesController::referrers, auth<1>::Type>()||
			
			_root/"poll"/"index"		<< _action<StatsController::views, auth<0>::Type>() ||
			_root/"stats"/"views"	<< _action<StatsController::views, auth<0>::Type>() ||
			
			_root/"poll"/"visitors"	<< _action<PagesController::Poll::visitors, auth<0>::Type>() ||
			_root/"poll"/"pages" << _action<PagesController::Poll::pages, auth<0>::Type>()||
			_root/"poll"/"referrers" << _action<PagesController::Poll::referrers, auth<0>::Type>() ||
			_root/"poll"/"account"/"time" << _action<PagesController::Poll::Account::time, auth<0>::Type>() ||
			
			_root/"account"/"change_password" << _action<PagesController::Poll::Account::change_password, auth<0>::Type>() ||
			_root/"account"/"add_website" << _action<PagesController::Poll::Account::add_website, auth<0>::Type>() ||
			
			//Admin panel
			_("admin")/"panel" << _action<PagesController::Admin::index, auth<0>::Type>() ||
			_("admin")/"activate" << _action<PagesController::Admin::activate, auth<0>::Type>() ||
			_("admin")/"impersonate" << _action<PagesController::Admin::impersonate, auth<0>::Type>()
			//_admin_root...
			/*
			_("lg")/"hit"			<< _action<LogController::hit>(),
			_("lg")/"click"			<< _action<LogController::click>(),
			_("lg")/"event"			<< _action<LogController::event>()
			*/, request);

	return action->handle(request);
}
