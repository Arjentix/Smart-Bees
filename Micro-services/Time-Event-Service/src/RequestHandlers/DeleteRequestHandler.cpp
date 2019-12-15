/**
 * @author: Polyakov Daniil
 * @mail: arjentix@gmail.com
 * @github: Arjentix
 * @date: 11.10.2019
*/

#include "DeleteRequestHandler.h"
#include <nlohmann/json.hpp>
#include <string>

using namespace RequestHandler;
using namespace std;
using json = nlohmann::json;

DeleteRequestHandler::DeleteRequestHandler(EventManager& event_manager)
	: RequestHandlerBase(event_manager) {}

HTTPHandler::Answer DeleteRequestHandler::handle(HTTPHandler::Request request) {
	string id = request.uri.substr(1);
	auto json_body = json::parse(request.body);

	_event_manager.remove(
		id,
		json_body["event_number"].get<size_t>()
	);

	return {
		200, "OK",
		{
			{"Content-Length", "0"}
		},
		""
	};
}
