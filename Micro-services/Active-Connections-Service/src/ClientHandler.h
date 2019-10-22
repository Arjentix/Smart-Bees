/**
 * @author: Polyakov Daniil
 * @mail: arjentix@gmail.com
 * @github: Arjentix
 * @date: 15.10.2019
*/

#pragma once

#include "RequestHandlerBase.h"
#include <unordered_map>
#include <utility>
#include <memory>

/**
 * ClientHandler - handles client's requests with handlers that have to be
 * provided by using add_request_handler() method.
 * Part of Comman Pattern.
 */
class ClientHandler {
public:
	void add_request_handler(
		HTTPHandler::Method method,
		std::shared_ptr<RequestHandler::RequestHandlerBase> handler_ptr
	);

	void handle_client(int client_sock) const;

private:
	std::unordered_map<
		HTTPHandler::Method,
		std::shared_ptr<RequestHandler::RequestHandlerBase>
	> _request_handlers;
};
