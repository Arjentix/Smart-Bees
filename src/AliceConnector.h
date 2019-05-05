/*
* Polyakov Daniil
* Mail: arjentix@gmail.com
* Github: Arjentix
* Date: 05.05.19
*/

#ifndef ALICE_CONNECTOR_H
#define ALICE_CONNECTOR_H

/*
* This file describes AliceConnector class. This class provides connection to
* my Alice skill and does all communtication with it.
*/

#include <string>

class AliceConnector {
private:
	bool _connected;
	int _sockfd;

public:
	/*
	* AliceConnector() - simply connects to the server with address and port form parameters.
	* Can throw std::runtime_error if can't establish connection with Alice server.
	*/
	AliceConnector(std::string address, int port);

	~AliceConnector();

	/*
	* get_token() - if connections was established waits for 2 seconds for arriving token from server.
	* Returns token if it was received and returns "" in another case.
	* Cat throw std::runtime_error if was being interrupted.
	*/
	std::string get_token();

	/*
	* send_ok() - sends OK-message to the Alice skill server.
	*/
	void send_ok();

	/*
	* send_err() - sends ERROR-message to the Alice skill server.
	*/
	void send_err();
};

#endif // ALICE_CONNECTOR_H
