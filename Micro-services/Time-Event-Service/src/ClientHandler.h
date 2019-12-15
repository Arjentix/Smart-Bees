/**
 * @author: Polyakov Daniil
 * @mail: arjentix@gmail.com
 * @github: Arjentix
 * @date: 11.10.2019
*/

#pragma once

#include "HTTPHandler.h"
#include "RequestHandlerBase.h"
#include <map>

class ClientHandler {
public:
	void add_request_handler(
		HTTPHandler::Method method,
		RequestHandler::RequestHandlerBase& handler
	);

	void handle_client(int client_sock) const;

private:
	std::map<
		HTTPHandler::Method,
		RequestHandler::RequestHandlerBase&
	> _request_handlers;
};
