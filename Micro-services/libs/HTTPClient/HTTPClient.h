
#pragma once
#include "HTTPHandler.h"
#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <netdb.h>
#include <sstream>


class HTTPClient {
private:

    int client;
    int res;
    struct sockaddr_in server_addr;
    socklen_t size;
    const int bufsize = 1024;
    bool connected;


public:
	//------ Exceptions ------
	struct ClientException : public std::runtime_error {
		ClientException(const std::string& what_msg);
	};

	struct RecvFailed : public ClientException {
		RecvFailed(const std::string& what_msg);
	};

	struct ServerDisconnected : public ClientException {
		ServerDisconnected(const std::string& what_msg);
	};

    HTTPClient();

    bool connect_to_server(std::string ip, int port);
    bool is_connected();
    void send_request(HTTPHandler::Request request);
    HTTPHandler::Answer read_answer();
	std::string read_raw();
    void close_conn();
    ~HTTPClient();
};

