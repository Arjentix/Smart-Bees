/**
 * @author: Polyakov Daniil
 * @mail: arjentix@gmail.com
 * @github: Arjentix
 * @date: 30.11.2019
 */

#include "CheckConnectionRequestHandler.h"
#include "Logger.h"
#include "nlohmann/json.hpp"
#include <string>

using namespace RequestHandler;
using namespace std;
using json = nlohmann::json;

CheckConnectionRequestHandler::CheckConnectionRequestHandler(ConnectionManager& connection_manager)
	: RequestHandlerBase(connection_manager) {}

HTTPHandler::Answer CheckConnectionRequestHandler::handle(HTTPHandler::Request request) {
	// Answer parameters
	json answer_body = json::object();
	int answer_code;
	string answer_code_descr;

	try {
		_connection_manager.check_connection(request.variables.at("gate_id"));
		answer_code = 200;
		answer_code_descr = "OK";
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
		logger << "CheckConnectionRequestHandler::Unexcpected exception: " << ex.what() << endl;
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
