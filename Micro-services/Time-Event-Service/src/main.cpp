/**
 * @author: Polyakov Daniil
 * @mail: arjentix@gmail.com
 * @github: Arjentix
 * @date: 09.10.2019
*/

#include "HTTPHandler.h"
#include "HTTPServer.h"
#include "EventManager.h"
#include "ClientHandler.h"
#include "GetRequestHandler.h"
#include "PostRequestHandler.h"
#include "DeleteRequestHandler.h"
#include <signal.h>
#include <vector>
#include <future>
#include <thread>

using namespace std;
using json = nlohmann::json;

bool finish = false;

void signal_handler()
{
	finish = true;
}

int main()
{
	HTTPServer server;
	server.start_server(3451);
	server.turn_to_listen(5);

	EventManager event_manager;

	// Capturing SIGINT signal
	signal(SIGINT, signal_handler);

	// Adding handlers
	ClientHandler client_handler;
	client_handler.add(
		HTTPHandler::Method::GET,
		RequestHandler::GetRequestHandler(event_manager)
	);
	client_handler.add(
		HTTPHandler::Method::POST,
		RequestHandler::PostRequestHandler(event_manager)
	);
	client_handler.add(
		HTTPHandler::Method::DELETE,
		RequestHandler::DeleteRequestHandler(event_manager)
	);

	vector<future<void>> futures;
	while (!finish) {
		int client_sock = server.connect_client();
		futures.push_back(async(
			client_handler.handle_client(client_sock);
		));
	}
	
	return 0;
}
