/**
 * @author: Polyakov Daniil
 * @mail: arjentix@gmail.com
 * @github: Arjentix
 * @date: 17.10.2019
 */

#pragma once

#include "RequestHandlerBase.h"

namespace RequestHandler {

	/**
	 * SendCommandRequestHandler sends given command to the gate identified by
	 * id.
	 * 
	 * Request should contains body with json text with fields:
	 * 		gate_id : string,
	 * 		command : string
	 * 
	 * Returns answer without body if the transfer was successful and answer
	 * with body if something went wrong. In this case body will contains field
	 * "error_message" with error describing
	 */

	class SendCommandRequestHandler : public RequestHandlerBase {
	public:
		SendCommandRequestHandler(ConnectionManager& connection_manager);

		HTTPHandler::Answer handle(HTTPHandler::Request request) override;
	};

};
