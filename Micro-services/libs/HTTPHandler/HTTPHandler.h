/**
 * @author: Polyakov Daniil
 * @mail: arjentix@gmail.com
 * @github: Arjentix
 * @date: 14.09.2019
*/

#pragma once

#include <string>
#include <ostream>
#include <map>
#include <vector>
#include <utility>

namespace HTTPHandler {
	/**
	 * Method - enumeration of possible HTTP methods,
	 * that can be parsed by parse_request() function.
	*/
	enum class Method {
		GET,
		POST,
		PUT,
		DELETE
	};

	/**
	 * Request - structure that describes a HTTP request.
	*/
	struct Request {
		Method method;
		std::string uri;
		std::map<std::string, std::string> variables;
		std::map<std::string, std::string> headers;
		std::string body;
	};

	/**
	 * Answer - structure that describes a HTTP answer.
	*/
	struct Answer {
		int status_code;
		std::string status_description;
		std::map<std::string, std::string> headers;
		std::string body;
	};

	/**
	 * parse_request() - parses HTTP request.
	 * Can throw std::invalid_argument if request format isn't allowed.
	*/
	Request parse_request(const std::string& request);

	/**
	 * write_request() - writes HTTP request to the given output stream.
	*/
	void write_request(const Request& request, std::ostream& output);

	/**
	 * parse_answer() - parses HTTP answer.
	 * Can throw std::invalid_argument if request format isn't allowed.
	*/
	Answer parse_answer(const std::string& answer);

	/**
	 * write_answer() - writes HTTP answer to the given output stream.
	*/
	void write_answer(const Answer& answer, std::ostream& output);
};
