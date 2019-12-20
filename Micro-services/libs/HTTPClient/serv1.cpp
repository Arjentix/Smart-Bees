
#include <iostream>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include "HTTPClient.h"


using namespace std;

int main()
{
    int server;
    int portNum = 1500;
    int bufsize = 1024;
    char buffer[bufsize];
    HTTPClient http_client;

    http_client.connect_to_server("127.0.0.1", portNum);
    
    http_client.send_request({
        HTTPHandler::Method::GET, "/wiki/page",
        {},
        {
            {"Connection", "close"},
            {"Host", "localhost"}
        },
        ""
    });

    auto answer = http_client.read_answer();

    http_client.close_conn();

    ostringstream oss;
    HTTPHandler::write_answer(answer, oss);
    cout << "Answer: " << oss.str() << endl;

    return 0;
}

