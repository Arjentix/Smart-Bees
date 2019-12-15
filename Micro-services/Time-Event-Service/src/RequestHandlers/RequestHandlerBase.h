/**
 * @author: Polyakov Daniil
 * @mail: arjentix@gmail.com
 * @github: Arjentix
 * @date: 11.10.2019
*/

#pragma once

#include "HTTPHandler.h"
#include "EventManager.h"

namespace RequestHandler {

	class RequestHandlerBase {
	public:
		RequestHandlerBase(EventManager& event_manager);

		virtual HTTPHandler::Answer handle(HTTPHandler::Request request) = 0;

	protected:
		EventManager& _event_manager;
	};

};
