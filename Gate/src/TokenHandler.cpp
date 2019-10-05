/*
* Polyakov Daniil
* Mail: arjentix@gmail.com
* Github: Arjentix
* Date: 04.05.19
*/

#include "TokenHandler.h"
#include <stdexcept>
#include <cstdio>
#include <cstring>

/*
* This file defines class TokenHandler decribed in TokenHandler.h.
* See all methods documentation in the header file.
*/

TokenHandler::TokenHandler(const std::string& base_path)
	: _base(std::ifstream(base_path))
{
	if (!_base.is_open()) {
		throw std::runtime_error("Can't open token base file");
	}
}

TokenHandler::~TokenHandler()
{
	_base.close();
}

bool TokenHandler::find(
	const std::string& token, std::string &topic, std::string &command
)
{
	char line[256];
	char getted_token[64];
	char getted_topic[128];
	char getted_command[64];
	int len;

	while(_base.getline(line, 256)) {
		sscanf(
			line, "%[^:] : %[^:] : %[^\n]", 
			getted_token, getted_topic, getted_command
		);

		/* Deleting last whitespaces in the getted_token */
		len = strlen(getted_token);
		while (getted_token[len - 1] == ' ') {
			getted_token[len - 1] = '\0';
			len = strlen(getted_token);
		}

		/* Deleting last whitespaces in the getted_topic */
		len = strlen(getted_topic);
		while (getted_topic[len - 1] == ' ') {
			getted_topic[len - 1] = '\0';
			len = strlen(getted_topic);
		}

		/* Deleting last whitespaces in the getted_command */
		len = strlen(getted_command);
		while (getted_command[len - 1] == ' ') {
			getted_command[len - 1] = '\0';
			len = strlen(getted_command);
		}

		if (strcmp(getted_token, token.c_str()) == 0) {
			topic = getted_topic;
			command = getted_command;
			_base.seekg(0, _base.beg);
			return true;
		}
	}

	_base.clear();
	_base.seekg(0, _base.beg);
	return false;
}
