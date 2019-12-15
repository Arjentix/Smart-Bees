/**
 * @author: Polyakov Daniil
 * @mail: arjentix@gmail.com
 * @github: Arjentix
 * @date: 14.10.2019
*/

#include "ConfigReader.h"
#include "Logger.h"
#include "HTTPServer.h"
#include "ConnectionManager.h"
#include "ClientHandler.h"
#include "SendCommandRequestHandler.h"
#include "CheckConnectionRequestHandler.h"
#include <unistd.h>
#include <signal.h>
#include <vector>
#include <future>
#include <thread>
#include <iostream>

using namespace std;

bool finish = false;

void signal_handler(int)
{
	logger << "Getted an interrupt signal" << endl;
	finish = true;
}

int main(int argc, char** argv)
{
	// Daemon mode
	if (argc > 1 && strcmp(argv[1], "-d") == 0) {
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
	}

	try {
		// Capturing SIGINT signal
		signal(SIGINT, signal_handler);

		ConfigReader::reader.set_file_name("config.txt");
		ConfigReader::reader.read_config(); // Buffering all configs

		logger.open(ConfigReader::reader.read_value_by_key<string>("LOG_FILE"));
		logger << "Start" << endl;

		HTTPServer server;
		server.start_server(
			ConfigReader::reader.read_value_by_key<int>("HTTP_SERVER_PORT")
		);
		server.turn_to_listen(5);

		ConnectionManager connection_manager(
			ConfigReader::reader.read_value_by_key<int>("GATE_SERVER_PORT")
		);

		// Adding handlers
		ClientHandler client_handler;
		client_handler.add_request_handler(
			{HTTPHandler::Method::POST, "/", {}},
			make_shared<
				RequestHandler::SendCommandRequestHandler
			>(connection_manager)
		);
		client_handler.add_request_handler(
			{HTTPHandler::Method::GET, "/connection", {"gate_id"}},
			make_shared<
				RequestHandler::CheckConnectionRequestHandler
			>(connection_manager)
		);

		logger << "Connecting" << endl;
		vector<future<void>> futures;
		while (!server.is_interrupted()) {
			int client_sock = server.connect_client();
			logger << "Client connected on socket " << client_sock << endl;
			futures.push_back(async(
				&ClientHandler::handle_client, &client_handler, client_sock
			));
		}
	}
	catch (HTTPServer::Interrupted& ex) {
		logger << ex.what() << endl;
	}
	catch (exception& ex) {
		logger << "Error: " << ex.what() << endl;
	}

	logger << "Shutting down" << endl;
	return 0;
}
