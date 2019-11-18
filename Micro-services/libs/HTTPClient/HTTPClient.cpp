
#include "HTTPClient.h"
#include <string.h>

using namespace HTTPHandler;

HTTPClient::ClientException::ClientException(const std::string& what_msg)
	: std::runtime_error(what_msg) {}

HTTPClient::RecvFailed::RecvFailed(const std::string& what_msg)
	: HTTPClient::ClientException(what_msg) {}

HTTPClient::ServerDisconnected::ServerDisconnected(const std::string& what_msg)
	: HTTPClient::ClientException(what_msg) {}

std::string get_n_bytes(int server, size_t n) {
	if (n <= 0) {
		return "";
	}

	char buffer[n + 1];	// + 1 for '\0'
	memset(buffer, 0, n + 1);

	int res = recv(server, buffer, n, 0); 
	if ( res == 0) {
		throw HTTPClient::ServerDisconnected(
			"Server disconnected on socket " + std::to_string(server)
		);
	}
	if (res < 0) {
		throw HTTPClient::RecvFailed(strerror(errno));
	}

	return buffer;
}

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
    // std::string answer;
    // char buffer[bufsize];
    // recv(client, buffer, bufsize, 0);
    // answer = buffer;
    // return HTTPHandler::parse_answer(answer);

	std::string raw_answer = read_raw();
	auto parsed_answer = HTTPHandler::parse_answer(raw_answer);

	size_t cont_len;
	if (parsed_answer.headers.count("Content-Length")) {
		cont_len = atoi(parsed_request.headers.at("Content-Length").c_str());
	}
	else if (parsed_answer.headers.count("content-length")) {
		cont_len = atoi(parsed_request.headers.at("content-length").c_str());
	}
	else {
		throw std::runtime_error("Expected Content-Length header");
	}

	parsed_answer.body += get_n_bytes(client, cont_len - parsed_request.body.size());

	return parsed_request
}

std::string HTTPClient::read_raw() {
	return get_n_bytes(client, bufsize);
}

void HTTPClient::close_conn(){
    close(client);
}
