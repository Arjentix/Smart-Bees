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

using namespace std;
using namespace HTTPHandler;

Method string_to_method(const string& method);
string method_to_string(Method method);
map<string, string> parse_variables(istream& input);
map<string, string> parse_headers(istream& input);

string Request::str() const
{
	ostringstream oss;
	write_request(*this, oss);

	return oss.str();
}

string Answer::str() const
{
	ostringstream oss;
	write_answer(*this, oss);

	return oss.str();
}

Request HTTPHandler::parse_request(const string& request)
{
	Request result;
	istringstream input(request);

	// Method
	string method_str;
	input >> method_str;
	result.method = string_to_method(method_str);

	// URI and variables
	input.ignore(1);
	string uri_and_vars;
	int pos;
	getline(input, uri_and_vars);
	if ((pos = uri_and_vars.find_first_of('?')) != uri_and_vars.npos) {
		result.uri = uri_and_vars.substr(0, pos);
		istringstream vars_stream(uri_and_vars);
		vars_stream.ignore(pos + 1, '?');
		result.variables = parse_variables(vars_stream);
	}
	else {
		result.uri = uri_and_vars.substr(0, uri_and_vars.find_first_of(' '));
	}

	//Headers
	result.headers = parse_headers(input);

	if (!input) {
		throw invalid_argument("Invalid request");
	}

	// Body
	stringstream body_stream;
	body_stream << input.rdbuf();
	result.body += body_stream.str();

	return result;
}

void HTTPHandler::write_request(const Request& request, std::ostream& output)
{
	output << method_to_string(request.method) << " " << request.uri;
	if (!request.variables.empty()) {
		output << "?";
		bool first = true;
		for (const auto& [var, val] : request.variables) {
			if (!first) {
				output << "&";
			}
			output << var << "=" << val;
			first = false;
		}

	}
	output << " HTTP/1.1\r\n";

	for (auto& [header, value] : request.headers) {
		output << header << ": " << value << "\r\n";
	}
	output << "\r\n" << request.body;
}

Answer HTTPHandler::parse_answer(const std::string& answer)
{
	Answer result;
	stringstream input(answer);

	input.ignore(256, ' ');

	input >> result.status_code >> result.status_description;
	
	input.ignore(256, '\n');
	result.headers = parse_headers(input);

	if (!input) {
		throw invalid_argument("Invalid answer");
	}

	stringstream body_stream;
	body_stream << input.rdbuf();
	result.body += body_stream.str();

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

Method string_to_method(const string& method)
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

string method_to_string(Method method)
{
	switch (method) {
	case (Method::GET) :
		return "GET";
		break;
	case (Method::POST) :
		return "POST";
		break;
	case (Method::PUT) :
		return "PUT";
		break;
	case (Method::DELETE) :
		return "DELETE";
		break;
	}

	throw invalid_argument("Unknown method");
}


map<string, string> parse_variables(istream& input)
{
	map<string, string> var_to_val;

	string var;
	while (getline(input, var, '=')) {
		string val;
		getline(input, val, '&');
		if (input.eof()) {
			val = val.substr(0, val.find_first_of(' '));
		}
		var_to_val[var] = val;
	}

	return var_to_val;
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
