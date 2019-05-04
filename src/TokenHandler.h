/*
* Polyakov Daniil
* Mail: arjentix@gmail.com
* Github: Arjentix
* Date: 04.05.19
*/

#ifndef TOKEN_HANDLER_H
#define TOKEN_HANDLER_H

/*
* This file describes TokenHandler class, which сhecks does token exist in base or not.
*/

#include <string>
#include <fstream>

class TokenHandler {
private:
	std::ifstream _base;

public:
	TokenHandler(std::string base_path);
	~TokenHandler();

	bool find(std::string token, std::string &topic, std::string &command);
};

#endif // TOKEN_HANDLER_H
