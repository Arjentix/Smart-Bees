#include "HTTPServer.h"

std::string get_n_bytes(int client, size_t n) {
	if (n <= 0) {
		return "";
	}

	char buffer[n + 1];	// + 1 for '\0'
	memset(buffer, 0, n + 1);

	recv(client, buffer, n, 0);
	return buffer;
}

HTTPServer::HTTPServer() {}

HTTPServer::~HTTPServer() {
	close(server);
//	for(auto&& thread: threads)
//		if(thread.joinable())
//			thread.join();
}

void HTTPServer::start_server(int port_num)
{
    server = socket(AF_INET, SOCK_STREAM, 0);

    if (server < 0) 
    {
		std::cout << "Error establishing socket..." << std::endl;
        exit(1);
    }

	std::cout << "Socket server has been created..." << std::endl;

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htons(INADDR_ANY);
    server_addr.sin_port = htons(port_num);

    if ((bind(server, (struct sockaddr*)&server_addr,sizeof(server_addr))) < 0) 
    {
		std::cout << "Error binding connection, the socket has already been established..." << std::endl;
		throw std::runtime_error("error on binding");
    }

    size = sizeof(server_addr);
}

void HTTPServer::turn_to_listen(int queue_size) {
	std::cout << "Looking for clients..." << std::endl;
    listen(server, queue_size);
}

int HTTPServer::connect_client()
{
    int client = accept(server,(struct sockaddr *)&server_addr, &size);

    if (client < 0) 
        std::cout << "Error on accepting..." << std::endl;
	else {
		std::cout << "Client was connected" << std::endl;
	}
	return client;
}

HTTPHandler::Request HTTPServer::get_request(int client) {
	std::string raw_request = get_raw(client);
	auto parsed_request = HTTPHandler::parse_request(raw_request);

	size_t cont_len;
	if (parsed_request.headers.count("Content-Length")) {
		cont_len = atoi(parsed_request.headers.at("Content-Length").c_str());
	}
	else if (parsed_request.headers.count("content-length")) {
		cont_len = atoi(parsed_request.headers.at("content-length").c_str());
	}
	else {
		throw std::runtime_error("Expected Content-Length header");
	}

	parsed_request.body += get_n_bytes(client, cont_len - parsed_request.body.size());

	return parsed_request;
}

std::string HTTPServer::get_raw(int client) {
	return get_n_bytes(client, bufsize);
}

void HTTPServer::send_answer(int client, const std::stringstream& answer_ss)
{
	static std::string buffer;
	buffer = answer_ss.str();
	send(client, buffer.c_str(), buffer.size(), 0);
}

void HTTPServer::close_con(int client)
{
    close(client);
}

/*
void HTTPServer::find_empty_thread(void (*f)(int), int client) {
	//check for empty threads
	for(auto& thread: threads) {
		if(thread.joinable())
			continue;
		thread = std::thread((*f)(client));
		return;
	}
	//if not, wait for one
	for(auto& thread: threads) {
		if(!thread.joinable())
			continue;
		thread.join();
		thread = std::thread((*f)(client));
		return;
	}
}
*/
