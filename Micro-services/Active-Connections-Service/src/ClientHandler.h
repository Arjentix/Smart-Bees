/**
 * @author: Polyakov Daniil
 * @mail: arjentix@gmail.com
 * @github: Arjentix
 * @date: 15.10.2019
*/

#pragma once

#include "RequestHandlerBase.h"
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <memory>

/**
 * ClientHandler - handles client's requests with handlers that have to be
 * provided by using add_request_handler() method.
 * Part of Comman Pattern.
 */
class ClientHandler {
public:
	/**
	 * RequestParams - structure of request parameters
	 */
	struct RequestParams {
		HTTPHandler::Method method;
		std::string url;
		std::vector<std::string> vars; // Variables for GET request

		bool operator==(const RequestParams& other) const;
	};

	void add_request_handler(
		RequestParams request_params,
		std::shared_ptr<RequestHandler::RequestHandlerBase> handler_ptr
	);

	void handle_client(int client_sock) const;

private:
	struct RequestParamsHash
	{
		std::size_t operator() (const RequestParams& params) const;
	};

	std::unordered_map<
		RequestParams,
		std::shared_ptr<RequestHandler::RequestHandlerBase>,
		RequestParamsHash
	> _request_handlers;

	std::unordered_set<HTTPHandler::Method> _available_methods;

	HTTPHandler::Answer _get_answer_for(const HTTPHandler::Request& request) const;
};
