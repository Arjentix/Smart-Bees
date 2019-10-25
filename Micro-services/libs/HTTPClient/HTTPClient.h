
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
    HTTPClient();

    bool connect_to_server(std::string ip, int port);
    bool is_connected();
    void send_request(HTTPHandler::Request request);
    HTTPHandler::Answer read_answer();
    void close_conn();
    ~HTTPClient();
};

