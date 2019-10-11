/**
 * @author: Polyakov Daniil
 * @mail: arjentix@gmail.com
 * @github: Arjentix
 * @date: 11.10.2019
*/

#pragma once

#include "RequestHandlerBase.h"

namespace RequestHandler {

	class PostRequestHandler : public RequestHandlerBase {
	public:
		PostRequestHandler(EventManager& event_manager);

		HTTPHandler::Answer handle(HTTPHandler::Request request) override;
	};

};
