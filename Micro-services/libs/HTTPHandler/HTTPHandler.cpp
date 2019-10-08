/**
 * @author: Polyakov Daniil
 * @mail: arjentix@gmail.com
 * @github: Arjentix
 * @date: 14.09.2019
*/

#include "HTTPHandler.h"

#include <sstream>
#include <stdexcept>
#include <algorithm>
#include <iostream>

using namespace std;
using namespace HTTPHandler;

Method parse_method(const string& method);
map<string, string> parse_headers(istream& input);
map<string, string> parse_body(istream& input);

Request HTTPHandler::parse_request(const string& request)
{
	std::cout << request << endl;
	Request result;
	istringstream input(request);

	string method_str;
	input >> method_str;
	result.method = parse_method(method_str);

	input >> result.uri;

	input.ignore(256, '\n');
	result.headers = parse_headers(input);

	if (!input) {
		throw invalid_argument("Expected empty line after headers");
	}

	result.body = parse_body(input);

	return result;
}

void HTTPHandler::write_answer(const Answer& answer, std::ostream& output)
{
	output << "HTTP/1.1 " << answer.status_code << " "
		   << answer.status_description << "\r\n";

	for (const auto& [key, value] : answer.headers) {
		output << key << ": " << value << "\r\n";
	}

	output << "\r\n" << answer.body;
}

Method parse_method(const string& method)
{
	if (method == "GET") {
		return Method::GET;
	}
	if (method == "POST") {
		return Method::POST;
	}
	if (method == "PUT") {
		return Method::PUT;
	}
	if (method == "DELETE") {
		return Method::DELETE;
	}

	throw invalid_argument("Unknown method: " + method);
}

map<string, string> parse_headers(istream& input)
{
	map<string, string> headers;

	string line;
	while (getline(input, line)) {
		if (line == "\r") {
			break;
		}

		istringstream cur_line(line);
		string key, value;
		getline(cur_line, key, ':');
		cur_line.ignore(1);
		getline(cur_line, value);
		value.erase(prev(value.end())); // Deleting '\r' charater

		headers[key] = value;
	}

	return headers;
}

map<string, string> parse_body(istream& input)
{
	map<string, string> body;

	string key;
	while (getline(input, key, '=')) {
		std::cout << "in while" << std::endl;
		string value;
		getline(input, value, '&');
		if (value.back() == '\n') {
			value.pop_back();
		}

		body[key] = value;
	}

	return body;
}
