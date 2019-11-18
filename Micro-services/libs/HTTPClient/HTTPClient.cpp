
#include "HTTPClient.h"
#include <string.h>

using namespace HTTPHandler;

HTTPClient::HTTPClient() : connected(false) {
      // Setting socket
    client = socket(AF_INET, SOCK_STREAM, 0);
    if (client < 0) {
        throw std::runtime_error("\nError establishing socket");
    }
}

HTTPClient::~HTTPClient(){
    close(client);
}

bool HTTPClient::connect_to_server(std::string ip, int port){
    
    // Setting server address
    sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    inet_pton(AF_INET, ip.c_str(), &server_addr.sin_addr);

    int res = connect(client,(struct sockaddr *)
            &server_addr, sizeof(server_addr));
    
    if(res == 0){
       std::cout << "=> Connection to the server "
        << inet_ntoa(server_addr.sin_addr) 
        << "port number: " << port << std::endl;
        connected = true;
    }
    
    return connected;
}

bool HTTPClient::is_connected(){
    return connected;
}

void HTTPClient::send_request(HTTPHandler::Request request){
    std::stringstream  req_stream;
    write_request(request, req_stream);

	std::string req_str = req_stream.str();
    send(client, req_str.c_str(), req_str.size(), 0);
}

HTTPHandler::Answer HTTPClient::read_answer(){
    std::string answer;
    char buffer[bufsize];
    recv(client, buffer, bufsize, 0);
    answer = buffer;
    return HTTPHandler::parse_answer(answer);
}

void HTTPClient::close_conn(){
    close(client);
}