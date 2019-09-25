#include "server.h"

Server::Server() {}

Server::~Server() {
	close(server);
}

void Server::start_server(int port_num)
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
        return;
    }

    size = sizeof(server_addr);
}

void Server::wait_for_con()
{
	std::cout << "=> Looking for clients..." << std::endl;
    listen(server, 1);

    client = accept(server,(struct sockaddr *)&server_addr, &size);

    if (client < 0) 
        std::cout << "=> Error on accepting..." << std::endl;
	std::cout << "Client was connected" << std::endl;
	get_request();
}

void Server::get_request()
{
	std::string str_request = "";
	while(recv(client, buffer, bufsize, 0))
	{
		std::cout << buffer;
		str_request += buffer;
		memset(buffer, 0, bufsize);
	}
	handling_request(str_request);
}

void Server::handling_request(const std::string& str_request)
{
	request = HTTPHandler::parse_request(str_request);
	//Here will be writing answer
	//send_answer(answer);
	
	send_answer();
}

void Server::send_answer()
{
	//send(client, answer, answer.size(), 0);
	close_con();
}

void Server::close_con()
{
	std::cout << "\n\n=> Connection terminated with IP " << inet_ntoa(server_addr.sin_addr) << std::endl;
    close(client);
}
