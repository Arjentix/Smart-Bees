/**
 * @author: Polyakov Daniil
 * @mail: arjentix@gmail.com
 * @github: Arjentix
 * @date: 15.10.2019
*/

#include "ConfigReader.h"
#include "ClientHandler.h"
#include "HTTPServer.h"
#include "Logger.h"
#include "nlohmann/detail/exceptions.hpp"
#include <string>

using namespace std;

void ClientHandler::add_request_handler(
	RequestParams request_params,
	std::shared_ptr<RequestHandler::RequestHandlerBase> handler_ptr
)
{
	_request_handlers.insert({request_params, handler_ptr});
	_available_methods.insert(request_params.method);
}

void ClientHandler::handle_client(int client_sock) const
{
	HTTPHandler::Answer http_answer;
	try {
		// Parsing request
		auto request = HTTPServer::get_request(client_sock);
		logger << "Getted request: \n" << request.str() << endl;

		http_answer = _get_answer_for(request);
	}
	catch (HTTPServer::ClientDisconnected& ex) {
		logger << ex.what() << endl;
		HTTPServer::close_con(client_sock);
		return;
	}

	logger << "Answer: \n" << http_answer.str() << endl;

	HTTPServer::send_answer(client_sock, http_answer);

	logger << "Closing connection with client on socket " << client_sock << endl;
	HTTPServer::close_con(client_sock);
}

HTTPHandler::Answer ClientHandler::_get_answer_for(
	const HTTPHandler::Request& request
) const
{
	const HTTPHandler::Answer bad_request_answer = {
		400, "Bad Request",
		{
			{"Content-Length", "0"},
			{"Connection", "close"}
		},
		""
	};

	try {
		// Checking for correct API key
		if (
			!request.headers.count("X-Api-Key") ||
			request.headers.at("X-Api-Key") !=
			ConfigReader::reader.read_value_by_key<string>("API_KEY")
		) {
			return {
				401, "Unauthorized",
				{
					{"Content-Length", "0"},
					{"Connection", "close"}
				},
				""
			};
		}

		vector<string> vars;
		for (const auto& [var, value] : request.variables) {
			vars.push_back(var);
		}
		RequestParams params = {
			request.method,
			request.uri,
			vars
		};
		// Checking for such HTTP request handler existence
		if (!_request_handlers.count(params)) {
			// Checking if any handler can process this type of HTTP method
			if (!_available_methods.count(request.method)) {
				return {
					405, "Method Not Allowed",
					{
						{"Content-Length", "0"},
						{"Connection", "close"}
					},
					""
				};
			}
			// There is a handler for given HTTP method but not for given URI
			return {
				404, "Not Found",
				{
					{"Content-Length", "0"},
					{"Connection", "close"}
				},
				""
			};
		}

		// Handling request and getting answer
		return _request_handlers.at(params)->handle(request);
	}
	catch (invalid_argument& ex) {
		logger << "invalid_argument: " << ex.what() << endl;
		return bad_request_answer;
	}
	catch(nlohmann::detail::exception& ex) {
		logger << "nlohmann::detail::exception: " << ex.what() << endl;
		return bad_request_answer;
	}
	catch (HTTPServer::ClientDisconnected& ex) {
		// If client disconnected when we shouldn't send something to this socket.
		// The calling function should care about such errors, so just throwing it to the higher level.
		throw ex;
	}
	catch (exception& ex) {
		logger << "exception: id = " << typeid(ex).name() <<  ", what = " << ex.what() << endl;
		return {
			500, "Internal Server Error",
			{
				{"Content-Length", "0"},
				{"Connection", "close"}
			},
			""
		};
	}
}



bool ClientHandler::RequestParams::operator==(const RequestParams& other) const
{
	return (
		make_tuple(method, url, vars) == 
		make_tuple(other.method, other.url, other.vars)
	);
}


size_t ClientHandler::RequestParamsHash::operator()(
	const RequestParams& params
) const
{
	size_t h = hash<int>()(static_cast<int>(params.method)) ^ hash<string>()(params.url);
	for (const auto& var : params.vars) {
		h ^= hash<string>()(var);
	}

	return h;
}
