/**
 * @author: Polyakov Daniil
 * @mail: arjentix@gmail.com
 * @github: Arjentix
 * @date: 11.10.2019
*/

#include "PostRequestHandler.h"
#include <nlohmann/json.hpp>
#include <string>

using namespace RequestHandler;
using namespace std;
using json = nlohmann::json;

PostRequestHandler::PostRequestHandler(EventManager& event_manager)
	: RequestHandlerBase(event_manager) {}

HTTPHandler::Answer PostRequestHandler::handle(HTTPHandler::Request request) {
	string id = request.uri.substr(1);
	auto json_body = json::parse(request.body);

	_event_manager.add(
		id,
		json_body.get<Event>()
	);

	return {
		200, "OK",
		{
			{"Content-Length", "0"}
		},
		""
	};
}
