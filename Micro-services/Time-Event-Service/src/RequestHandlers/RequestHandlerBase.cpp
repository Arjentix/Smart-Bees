/**
 * @author: Polyakov Daniil
 * @mail: arjentix@gmail.com
 * @github: Arjentix
 * @date: 11.10.2019
*/

#include "RequestHandlerBase.h"

using namespace RequestHandler;

RequestHandlerBase::RequestHandlerBase(EventManager& event_mamanger)
	: _event_manager(event_mamanger) {}
