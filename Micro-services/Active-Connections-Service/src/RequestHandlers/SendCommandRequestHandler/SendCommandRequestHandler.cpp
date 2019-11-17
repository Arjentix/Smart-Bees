/**
 * @author: Polyakov Daniil
 * @mail: arjentix@gmail.com
 * @github: Arjentix
 * @date: 15.10.2019
 */

#include "SendCommandRequestHandler.h"
#include "Logger.h"
#include "nlohmann/json.hpp"
#include <string>

using namespace RequestHandler;
using namespace std;
using json = nlohmann::json;

SendCommandRequestHandler::SendCommandRequestHandler(ConnectionManager& connection_manager)
	: RequestHandlerBase(connection_manager) {}

HTTPHandler::Answer SendCommandRequestHandler::handle(HTTPHandler::Request request) {
	auto json_body = json::parse(request.body);

	// Answer parameters
	json answer_body = json::object();
	int answer_code;
	string answer_code_descr;

	try {
		string gate_answer = _connection_manager.send_command(
			json_body.at("gate_id").get<string>(),
			json_body.at("command").get<string>()
		);
		logger << "Gate answer = '" << gate_answer << "'" << endl;
		if (gate_answer == "OK") {
			answer_code = 200;
			answer_code_descr = "OK";
		}
		else {	// Gate can't process this command
			answer_code = 412;
			answer_code_descr = "Misdirected Request";
			answer_body = {{"error_message", "Неизвестная команда"}};
		}
	}
	catch (out_of_range& ex) {
		answer_code = 404;
		answer_code_descr = "Not Found";
		answer_body = {{"error_message", "Нет такого шлюза"}};
	}
	catch (HTTPServer::SendFailed& ex) {
		answer_code = 503;
		answer_code_descr = "Service Unavailable";
		answer_body = {{"error_message", "Не удалось выполнить отправку на шлюз"}};
	}
	catch (HTTPServer::RecvFailed& ex) {
		answer_code = 503;
		answer_code_descr = "Service Unavailable";
		answer_body = {{"error_message", "Не удалось получить ответ от шлюза"}};
	}
	catch (HTTPServer::ClientDisconnected& ex) {
		answer_code = 503;
		answer_code_descr = "Service Unavailable";
		answer_body = {{"error_message", "Потеряно соединение со шлюзом"}};
	}
	catch (exception& ex) {
		logger << "SendCommandRequestHandler::Unexcpected exception: " << ex.what() << endl;
		answer_code = 520;
		answer_code_descr = "Unknown Error";
		answer_body = {{"error_message", "Неизвестная ошибка"}};
	}

	string answer_body_str = answer_body.dump(4);
	if (answer_body_str == "{}") {
		answer_body_str = "";
	}
	return {
		answer_code, answer_code_descr,
		{
			{"Connection", "close"},
			{"Content-Type", "application/json"},
			{"Content-Length", to_string(answer_body_str.size())},
		},
		answer_body_str
	};
}
