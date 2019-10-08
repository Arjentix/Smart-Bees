#include "HTTPServer.h"

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
		std::cout << "\nError establishing socket..." << std::endl;
        exit(1);
    }

	std::cout << "\n=> Socket server has been created..." << std::endl;

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htons(INADDR_ANY);
    server_addr.sin_port = htons(port_num);

    if ((bind(server, (struct sockaddr*)&server_addr,sizeof(server_addr))) < 0) 
    {
		std::cout << "=> Error binding connection, the socket has already been established..." << std::endl;
		throw std::runtime_error("error on binding");
    }

    size = sizeof(server_addr);
}

void HTTPServer::turn_to_listen(int queue_size) {
	std::cout << "=> Looking for clients..." << std::endl;
    listen(server, queue_size);
}

int HTTPServer::connect_client()
{
    int client = accept(server,(struct sockaddr *)&server_addr, &size);

    if (client < 0) 
        std::cout << "=> Error on accepting..." << std::endl;
	else {
		std::cout << "Client was connected" << std::endl;
	}
	return client;
}

std::string HTTPServer::get_request(int client) {
	std::string str_request = "";
	recv(client, buffer, bufsize, 0);
		str_request += buffer;
		memset(buffer, 0, bufsize);
	return str_request;
}

HTTPHandler::Request HTTPServer::handling_request(const std::string& str_request) {
	HTTPHandler::Request request = HTTPHandler::parse_request(str_request);
	return request;
}

void HTTPServer::send_answer(int client, HTTPHandler::Answer answer)
{
	std::stringstream answer_ss;
	HTTPHandler::write_answer(answer, answer_ss);
	while(answer_ss) {
		answer_ss.get(buffer, bufsize);
		send(client, buffer, bufsize, 0);
	}
}

void HTTPServer::close_con(int client)
{
	std::cout << "\n\n=> Connection terminated with IP " << inet_ntoa(server_addr.sin_addr) << std::endl;
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
