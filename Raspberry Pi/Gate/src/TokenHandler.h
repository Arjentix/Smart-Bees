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
	/*
	* TokenHanlder() - opens file with path base_path.
	* Can throw str::runtime_error if can't open file.
	*/
	TokenHandler(const std::string& base_path);

	~TokenHandler();

	/*
	* find() - searches in base for given token. In succes it plases relevant
	* topic and command into the given parameters and return true.
	* In another case returns false.
	*/
	bool find(const std::string& token, std::string &topic, std::string &command);
};

#endif // TOKEN_HANDLER_H
