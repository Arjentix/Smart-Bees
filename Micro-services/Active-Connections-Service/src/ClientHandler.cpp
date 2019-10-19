/**
 * @author: Polyakov Daniil
 * @mail: arjentix@gmail.com
 * @github: Arjentix
 * @date: 15.10.2019
*/

#include "ClientHandler.h"
#include "HTTPServer.h"
#include "Logger.h"
#include "nlohmann/detail/exceptions.hpp"
#include <string>

using namespace std;

void ClientHandler::add_request_handler(
	HTTPHandler::Method method,
	std::shared_ptr<RequestHandler::RequestHandlerBase> handler_ptr
)
{
	_request_handlers.insert({method, handler_ptr});
}

void ClientHandler::handle_client(int client_sock) const
{
	HTTPHandler::Answer http_answer = {	// Default answer
		400, "Bad Request",
		{
			{"Content-Length", "0"},
			{"Connection", "close"}
		},
		""
	};

	string request_str;
	try {
		// Parsing request
		auto request = HTTPServer::get_request(client_sock);
		logger << "Getted request body: \n" << request.body << endl;

		// If there is a handler for this HTTP method
		if (_request_handlers.count(request.method)) {
			// Handling request and getting answer
			http_answer = _request_handlers.at(request.method)
							->handle(request);
		}
		else {
			http_answer = {
				405, "Method Not Allowed",
				{
					{"Content-Length", "0"},
					{"Connection", "close"}
				},
				""
			};
		}
	}
	catch (out_of_range& ex) {
		http_answer.status_code = 404;
		http_answer.status_description = "Not Found";
		logger << "out_of_range: " << ex.what() << endl;
	}
	catch (invalid_argument& ex) {
		// http_answer is already set to "Bad Request"
		logger << "invalid_argument: " << ex.what() << endl;
	}
	catch(nlohmann::detail::exception& ex) {
		// http_answer is already set to "Bad Request"
		logger << "nlohmann::detail::exception: " << ex.what() << endl;
	}
	catch (exception& ex) {
		logger << "exception: id = " << typeid(ex).name() <<  ", what = " << ex.what() << endl;
		http_answer = {
			500, "Internal Server Error",
			{
				{"Content-Length", "0"},
				{"Connection", "close"}
			},
			""
		};
	}

	// Sending answer
	stringstream answer_stream;
	HTTPHandler::write_answer(http_answer, answer_stream);
	logger << "Answer: \n" << answer_stream.str() << endl;
	HTTPServer::send_answer(client_sock, http_answer);

	logger << "Closing connection with client on socket " << client_sock << endl;
	HTTPServer::close_con(client_sock);
}
