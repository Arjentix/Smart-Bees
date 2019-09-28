#pragma once

#include <iostream>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
//#include "HTTPHandler.h"
#include <thread>
#include <vector>

class Server {
private:
	int server;
    struct sockaddr_in server_addr;
    socklen_t size;
    const static int bufsize = 1024;
    char buffer[bufsize];
//	HTTPHandler::Request request;

public:
	Server();
	~Server();
	void start_server(int);
	void turn_to_listen(int);
	void connect_client();
	void get_request(int);
	void handling_request(const std::string&);
	void send_answer();
	void close_con();
};
