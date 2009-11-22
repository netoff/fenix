#include "fenix.h"
#include "hello_controller.h"

using namespace fenix::web::toolkit;

FENIX_APPLICATION(HelloWorld)
{
	auto_ptr<Action> action = dispatch(
			_("app")					<< _action<HelloController::index>()||
			_("app")/_("index")		<< _action<HelloController::index>(), request);

	return action->handle(request);
}
