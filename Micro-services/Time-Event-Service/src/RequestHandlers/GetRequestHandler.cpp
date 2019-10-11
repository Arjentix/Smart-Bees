/**
 * @author: Polyakov Daniil
 * @mail: arjentix@gmail.com
 * @github: Arjentix
 * @date: 11.10.2019
*/

#include "GetRequestHandler.h"
#include <nlohmann/json.hpp>
#include <string>

using namespace RequestHandler;
using namespace std;
using json = nlohmann::json;

GetRequestHandler::GetRequestHandler(EventManager& event_manager)
	: RequestHandlerBase(event_manager) {}

HTTPHandler::Answer GetRequestHandler::handle(HTTPHandler::Request request) {
	string id = request.uri.substr(1);
	json j_array(_event_manager.get_events_for(id));

	string json_str = j_array.dump(4);

	return {
		200, "OK",
		{
			{"Content-Type", "application/json"},
			{"Content-Length", to_string(json_str.length())}
		}, 
		json_str
	};
}
