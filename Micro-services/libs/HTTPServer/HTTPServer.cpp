#include "HTTPServer.h"

HTTPServer::Exception::Exception(const std::string& what_msg)
	: std::runtime_error(what_msg) {}

HTTPServer::SocketFailed::SocketFailed(const std::string& what_msg)
	: HTTPServer::Exception(what_msg) {}

HTTPServer::BindFailed::BindFailed(const std::string& what_msg)
	: HTTPServer::Exception(what_msg) {}

HTTPServer::AcceptFailed::AcceptFailed(const std::string& what_msg)
	: HTTPServer::Exception(what_msg) {}

HTTPServer::RecvFailed::RecvFailed(const std::string& what_msg)
	: HTTPServer::Exception(what_msg) {}

HTTPServer::SendFailed::SendFailed(const std::string& what_msg)
	: HTTPServer::Exception(what_msg) {}


std::string get_n_bytes(int client, size_t n) {
	if (n <= 0) {
		return "";
	}

	char buffer[n + 1];	// + 1 for '\0'
	memset(buffer, 0, n + 1);

	if (recv(client, buffer, n, 0) <= 0) {
		throw HTTPServer::RecvFailed(strerror(errno));
	}
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
		throw HTTPServer::SocketFailed(strerror(errno));
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htons(INADDR_ANY);
    server_addr.sin_port = htons(port_num);

    if ((bind(server, (struct sockaddr*)&server_addr,sizeof(server_addr))) < 0) 
    {
		throw HTTPServer::BindFailed(strerror(errno));
    }

    size = sizeof(server_addr);
}

void HTTPServer::turn_to_listen(int queue_size) {
    listen(server, queue_size);
}

int HTTPServer::connect_client()
{
    int client = accept(server,(struct sockaddr *)&server_addr, &size);

    if (client < 0) {
		throw AcceptFailed(strerror(errno));
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

void HTTPServer::send_answer(int client, const HTTPHandler::Answer& answer)
{
	std::stringstream answer_ss;
	HTTPHandler::write_answer(answer, answer_ss);
	send_raw(client, answer_ss.str());
}

void HTTPServer::send_raw(int client, const std::string& answer_str)
{
	if (send(client, answer_str.c_str(), answer_str.size(), 0) < 0)
	{
		throw SendFailed(strerror(errno));
	}
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
