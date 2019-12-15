/**
 * @author: Polyakov Daniil
 * @mail: arjentix@gmail.com
 * @github: Arjentix
 * @date: 30.11.2019
 */

#pragma once

#include "RequestHandlerBase.h"

namespace RequestHandler {

	/**
	 * CheckConnectionRequestHandler checks for connection with gate identified
	 * by the given id. Requires "id" variable.
	 */

	class CheckConnectionRequestHandler : public RequestHandlerBase {
	public:
		CheckConnectionRequestHandler(ConnectionManager& connection_manager);

		HTTPHandler::Answer handle(HTTPHandler::Request request) override;
	};

};
