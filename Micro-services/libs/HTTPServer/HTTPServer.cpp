#include "HTTPServer.h"
#include <signal.h>
#include <sys/select.h>

HTTPServer::ServerException::ServerException(const std::string& what_msg)
	: std::runtime_error(what_msg) {}

HTTPServer::SocketFailed::SocketFailed(const std::string& what_msg)
	: HTTPServer::ServerException(what_msg) {}

HTTPServer::BindFailed::BindFailed(const std::string& what_msg)
	: HTTPServer::ServerException(what_msg) {}

HTTPServer::AcceptFailed::AcceptFailed(const std::string& what_msg)
	: HTTPServer::ServerException(what_msg) {}

HTTPServer::RecvFailed::RecvFailed(const std::string& what_msg)
	: HTTPServer::ServerException(what_msg) {}

HTTPServer::ClientDisconnected::ClientDisconnected(const std::string& what_msg)
	: HTTPServer::ServerException(what_msg) {}

HTTPServer::SendFailed::SendFailed(const std::string& what_msg)
	: HTTPServer::ServerException(what_msg) {}

HTTPServer::Interrupted::Interrupted(const std::string& what_msg)
	: HTTPServer::ServerException(what_msg) {}

bool HTTPServer::_interrupted = false;
std::mutex HTTPServer::_locker;

/**
 * _wait_for_data() -- call pselect system call which is waiting for input data.
 * Can throw HTTPServer::Interrupted excpetion.
 */
int HTTPServer::_wait_for_data(int sock) {
	fd_set		inputs;
	timespec	timeout;

	// Setting select parameters
	FD_ZERO(&inputs);
	FD_SET(sock, &inputs);
	timeout.tv_sec = 2;
	timeout.tv_nsec = 0;

	// Infinite loop
	// Returns when there are some data on the given socket
	while(!is_interrupted()) {
		int res = pselect(
			FD_SETSIZE, &inputs, (fd_set*)NULL,
			(fd_set*)NULL, &timeout, (sigset_t*)NULL
		);
		if (res < 0) {			// Select failed
			set_interrupted(true);
			throw HTTPServer::Interrupted("Select was interrupted");
		}
		// else if (res == 0) {	// No input
		// 	continue;
		// }
		// There is an input data
		if (FD_ISSET(sock, &inputs)) {
			return res;
		}
	}

	throw HTTPServer::Interrupted("Select was interrupted");
}

std::string HTTPServer::get_n_bytes(int client, size_t n) {
	if (n <= 0) {
		return "";
	}

	char buffer[n + 1];	// + 1 for '\0'
	memset(buffer, 0, n + 1);

	_wait_for_data(client);
	int res = recv(client, buffer, n, 0); 
	if (res == 0) {
		throw HTTPServer::ClientDisconnected(
			"Client disconnected on socket " + std::to_string(client)
		);
	}
	if (res < 0) {
		throw HTTPServer::RecvFailed(strerror(errno));
	}

	return buffer;
}

HTTPServer::HTTPServer()
{
	signal(SIGPIPE, SIG_IGN);
}

HTTPServer::~HTTPServer() {
	close(server);
//	for(auto&& thread: threads)
//		if(thread.joinable())
//			thread.join();
}

bool HTTPServer::is_interrupted() {
	return _interrupted;
}

void HTTPServer::set_interrupted(bool flag) {
	std::lock_guard guard(_locker);
	_interrupted = flag;
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

	int opt = 1;
	setsockopt (server, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof (opt));
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
	// _wait_for_data(server);
	int client = accept(server, (struct sockaddr *)&server_addr, &size);
	if (client < 0) {
		throw AcceptFailed(strerror(errno));
	}

	return client;
}

HTTPHandler::Request HTTPServer::get_request(int client) {
	std::string raw_request = get_raw(client);
	auto parsed_request = HTTPHandler::parse_request(raw_request);

	size_t cont_len;
	if (
		parsed_request.method == HTTPHandler::Method::POST ||
		parsed_request.method == HTTPHandler::Method::PUT
	) {
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
	}

	return parsed_request;
}

std::string HTTPServer::get_raw(int client) {
	return get_n_bytes(client, bufsize);
}

void HTTPServer::send_answer(int client, const HTTPHandler::Answer& answer)
{
	std::stringstream answer_ss;
	HTTPHandler::write_answer(answer, answer_ss);
	send_raw(client, answer_ss.str(), false);
}

void HTTPServer::send_raw(
	int client,
	const std::string& answer_str,
	bool include_null_ch
)
{
	int increment = (include_null_ch ? 1 : 0);
	if (send(client, answer_str.c_str(), answer_str.size() + increment, 0) < 0)
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
