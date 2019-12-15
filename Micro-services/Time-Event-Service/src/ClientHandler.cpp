/**
 * @author: Polyakov Daniil
 * @mail: arjentix@gmail.com
 * @github: Arjentix
 * @date: 11.10.2019
*/

#include "ClientHandler.h"
#include "HTTPServer.h"
#include <string>

using namespace std;

void ClientHandler::add_request_handler(
	HTTPHandler::Method method,
	RequestHandler::RequestHandlerBase& handler
)
{
	_request_handlers.insert({method, handler});
}

void handle_client(int client_sock) const
{
	HTTPHandler::Answer http_answer;	// Result answer
	string request_str;
	while ((request_str = HTTPServer::get_request(client_sock)) != "") {
		try {
			// Parsing request
			auto request = HTTPHandler::parse_request(
				HTTPServer::get_request(client_sock)
			);

			// If there is a handler for this HTTP method
			if (_request_handlers.count(request.method)) {
				// Handling request and getting answer
				http_answer = _request_handlers.at(request.method)
								.handle(request);
			}
			else {
				http_answer = {
					405, "Method Not Allowed",
					{
						{"Content-Length", "0"}
					},
					""
				};
			}
		}
		catch (invalid_argument& ex) {
			http_answer = {
				400, "Bad Request",
				{
					{"Content-Length", "0"}
				},
				""
			};
		}
		catch (...) {
			http_answer = {
				500, "Internal Server Error",
				{
					{"Content-Length", "0"}
				},
				""
			}
		}

		// Sending answer
		stringstream answer_stream;
		HTTPHandler::write_answer(http_answer, answer_stream);
		HTTPServer::send_answer(cliet_sock, answer_stream);
	}

	HTTPServer::close_con(client_sock);
}
