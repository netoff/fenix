#include "fenix.h"
#include "hello_controller.h"
#include "dashboard_controller.h"
#include "stats_controller.h"

using namespace fenix::web::toolkit;

FENIX_APPLICATION(HelloWorld)
{
	auto_ptr<Action> action = dispatch(
			_("app")						<< _action<HelloController::index>() ||
			_("app")/_("index")				<< _action<DashboardController::index>() ||
			_("app")/_("stats")/_("views")	<< _action<StatsController::views>(), request);

	return action->handle(request);
}
