#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include <stdexcept>
#include <exception>

using namespace std;

int main()
{
	try {
		/* Initialization */
		int server_sockfd;
		int client_sockfd;
		struct sockaddr_in server_address;
		char buffer[512];
		int res;

		/* Creating socket */
		server_sockfd = socket(AF_INET, SOCK_STREAM, 0);

		/* Setting address */
		server_address.sin_family = AF_INET;
		server_address.sin_addr.s_addr = htonl(INADDR_ANY);
		server_address.sin_port = htons(4551);

		/* Binding */
		res = bind(server_sockfd, (struct sockaddr*)&server_address,
				sizeof(server_address));
		if (res != 0) {
			throw std::runtime_error("Failed to bind socket");
		}

		/* Listening */
		listen(server_sockfd, 5);

		/* Accepting */
		client_sockfd = accept(server_sockfd, (struct sockaddr*)NULL,
					(socklen_t*)NULL);
		if (client_sockfd == -1) {
			throw std::runtime_error("Failed to accept connection with client");
		}

		cout << "Connected to the client" << endl;
		do {
			cout << "Enter token: ";
			cin.getline(buffer, 512);
			send(client_sockfd, buffer, 512, 0);
			recv(client_sockfd, buffer, 512, 0);
			cout << "Answer: " << buffer << endl;
		} while (strcmp(buffer, "EOF") != 0);
	}
	catch (const std::exception &error) {
		cout << error.what() << endl;
		return -1;
	}
}
