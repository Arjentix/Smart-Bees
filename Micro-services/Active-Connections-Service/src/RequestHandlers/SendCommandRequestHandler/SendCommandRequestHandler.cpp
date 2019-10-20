/**
 * @author: Polyakov Daniil
 * @mail: arjentix@gmail.com
 * @github: Arjentix
 * @date: 15.10.2019
 */

#include "SendCommandRequestHandler.h"
#include "Logger.h"
#include "json.hpp"
#include <string>

using namespace RequestHandler;
using namespace std;
using json = nlohmann::json;

SendCommandRequestHandler::SendCommandRequestHandler(ConnectionManager& connection_manager)
	: RequestHandlerBase(connection_manager) {}

HTTPHandler::Answer SendCommandRequestHandler::handle(HTTPHandler::Request request) {
	auto json_body = json::parse(request.body);

	try {
		_connection_manager.send_command(
			json_body.at("gate_id").get<string>(),
			json_body.at("command").get<string>()
		);
	}
	catch (out_of_range& ex) {
		return {
			404, "Not Found",
			{
				{"Content-Length", "0"},
				{"Connection", "close"}
			},
			""
		};
	}
	catch (HTTPServer::SendFailed& ex) {
		logger << "SendCommandRequestHandler::Send failed: " << ex.what() << endl;
		return {
			503, "Service Unavailable",
			{
				{"Content-Length", "0"},
				{"Connection", "close"}
			},
			""
		};
	}
	catch (exception& ex) {
		logger << "SendCommandRequestHandler::Unexcpected exception: " << ex.what() << endl;
		return {
			520, "Unknown Error",
			{
				{"Content-Length", "0"},
				{"Connection", "close"}
			},
			""
		};
	}
	return {
		200, "OK",
		{
			{"Content-Length", "0"},
			{"Connection", "close"}
		},
		""
	};
}
