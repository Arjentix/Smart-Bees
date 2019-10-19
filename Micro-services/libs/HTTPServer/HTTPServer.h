#pragma once

#include "HTTPHandler.h"
#include <iostream>
#include <string.h>
#include <sstream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <thread>
#include <vector>
#include <exception>

class HTTPServer {
private:
	int server;
    struct sockaddr_in server_addr;
    socklen_t size;
    const static int bufsize = 1024;

public:
	HTTPServer();
	~HTTPServer();
	void start_server(int);
	void turn_to_listen(int);
	int connect_client();
	static HTTPHandler::Request get_request(int);
	static std::string get_raw(int);
	static void send_answer(int, const std::stringstream&);
	static void close_con(int);
};
