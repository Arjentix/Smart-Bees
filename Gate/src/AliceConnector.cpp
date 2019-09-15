/*
* Polyakov Daniil
* Mail: arjentix@gmail.com
* Github: Arjentix
* Date: 09.03.19
*/

#include "AliceConnector.h"
#include "LogPrinter.h"
#include <stdexcept>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <cstring>

/*
* BUF_SIZE - size of the buffer for receiving and sending messages to Alice.
*/
#define BUF_SIZE 512

/*
* This file defines AliceConnector class, which is described in the AliceConnector.h.
* See all methods documentation in the header file.
*/

AliceConnector::AliceConnector(std::string address, int port) : _connected(false),
_sockfd(socket(AF_INET, SOCK_STREAM, 0))
{
	/* Initialization */
	struct sockaddr_in	socket_address;
	int			result;
	hostent			*host;

	/* Setting address */
	socket_address.sin_family = AF_INET;
	host = gethostbyname(address.c_str());
	if (host == NULL) {
		throw std::runtime_error("Failed to get host");
	}
	memcpy(&socket_address.sin_addr, host->h_addr_list[0], host->h_length);
	socket_address.sin_port = htons(port);

	/* Connecting */
	LogPrinter::print("Connecting");
	result = connect(_sockfd, (struct sockaddr *)&socket_address, sizeof(socket_address));
	if (result == -1) {
		throw std::runtime_error("Failed to connect to Alice.");
	}
	LogPrinter::print("Connected");

	/* Sending serial number */
	char buffer[BUF_SIZE] = {"12345\0"};
	send(_sockfd, buffer, strlen(buffer), 0);

	_connected = true;
}

AliceConnector::~AliceConnector()
{
	close(_sockfd);
	_connected == false;
}

std::string AliceConnector::get_token()
{
	if (!_connected) {
		return "";
	}

	/* Initialization */
	char		buffer[BUF_SIZE];
	fd_set		inputs;
	struct timespec	timeout;
	int		res;

	memset(buffer, BUF_SIZE, 0);

	/* Setting select parameters */
	FD_ZERO(&inputs);
	FD_SET(_sockfd, &inputs);
	timeout.tv_sec = 2;
	timeout.tv_nsec = 0;

	/* Waiting for token for timeout seconds */
	res = pselect(FD_SETSIZE, &inputs, (fd_set*)NULL,
			(fd_set*)NULL, &timeout, (const sigset_t*)NULL);
	switch(res) {
	case -1:	// Select failed
		throw std::runtime_error("Select was interrupted");
		break;
	case 0:		// No input token
		break;
	default:	// There is an input token
		if (FD_ISSET(_sockfd, &inputs)) {
			recv(_sockfd, buffer, BUF_SIZE, 0);
			return buffer;
		}
	}

	return "";
}

void AliceConnector::send_ok()
{
	if (!_connected) {
		return;
	}

	char buffer[BUF_SIZE] = {"OK\0"};
	send(_sockfd, buffer, BUF_SIZE, 0);
}

void AliceConnector::send_err()
{
	if (!_connected) {
		return;
	}

	char buffer[BUF_SIZE] = {"ERROR\0"};
	send(_sockfd, buffer, BUF_SIZE, 0);
}
