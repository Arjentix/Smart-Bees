/**
 * @author: Polyakov Daniil
 * @mail: arjentix@gmail.com
 * @github: Arjentix
 * @date: 14.10.2019
*/

#include "Logger.h"
#include "HTTPServer.h"
#include "ConnectionManager.h"
#include "ClientHandler.h"
#include "SendCommandRequestHandler.h"
#include <unistd.h>
#include <signal.h>
#include <vector>
#include <future>
#include <thread>

using namespace std;

bool finish = false;

void signal_handler(int)
{
	finish = true;
}

int main()
{
    int fork_res = fork();
    if (fork_res == -1) {	// Error
        return -1;
    }
    if (fork_res > 0) {		// Parent
        return 0;
    }
    
    // Child 
    // Closing useless fds
    close(0);
    close(1);
    close(2);

	try {
		logger << "Start" << endl;

		HTTPServer server;
		server.start_server(2525);
		server.turn_to_listen(5);

		logger << "Before manager" << endl;
		ConnectionManager connection_manager;
		logger << "After manager" << endl;

		// Capturing SIGINT signal
		signal(SIGINT, signal_handler);

		// Adding handlers
		ClientHandler client_handler;
		client_handler.add_request_handler(
			HTTPHandler::Method::POST,
			make_shared<
				RequestHandler::SendCommandRequestHandler
			>(connection_manager)
		);

		logger << "Connecting" << endl;
		vector<future<void>> futures;
		while (!finish) {
			int client_sock = server.connect_client();
			logger << "Client connected on socket " << client_sock << endl;
			futures.push_back(async(
				&ClientHandler::handle_client, &client_handler, client_sock
			));
		}
	}
	catch (exception& ex) {
		logger << "Error: " << ex.what() << endl;
	}
	
	logger << "Shutting down" << endl;
	return 0;
}
