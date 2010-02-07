#include "controllers.h"

using namespace fenix::web::toolkit;

FENIX_APPLICATION(HelloWorld)
{
	auto_ptr<Action> action = dispatch(
			_("app")			<< _action<HelloController::index>() ||
			_("app")/_("index")		<< _action<DashboardController::index>() ||
			_("app")/"index"/"poll"		<< _action<StatsController::views>() ||
			_("app")/_("stats")/_("views")	<< _action<StatsController::views>() ||
			_("lg")/"view"			<< _action<LogController::lg>() ||
			_("app")/"visitors"		<< _action<PagesController::visitors>() ||
			_("app")/"visitors"/"poll"	<< _action<PagesController::Poll::visitors>()
			/*
			_("app")/"pages"		<< _action<PagesController::pages>(),
			_("app")/"referrers"		<< _action<PagesController::referrers>(),
			_("lg")/"hit"			<< _action<LogController::hit>(),
			_("lg")/"click"			<< _action<LogController::click>(),
			_("lg")/"event"			<< _action<LogController::event>()
			*/, request);

	return action->handle(request);
}
