/*
* Polyakov Daniil
* Mail: arjentix@gmail.com
* Github: Arjentix
* Date: 11.03.19
*/

#include "LogPrinter.h"
#include <iostream>
#include <ctime>

/*
* This file defines methods of class LogPrinter,
* which is described in LogPrinter.h.
* See all interface description in the header file.
*/

std::ofstream LogPrinter::log_file = std::ofstream("log/log.txt", std::ios_base::out | std::ios_base::trunc);

/*
* get_time() - returns string with current date and time.
*/
std::string get_time()
{
	std::time_t now = time(0);
	std::tm *tm_struct;
	char buf[80];

	tm_struct = localtime(&now);
	strftime(buf, sizeof(buf), "%F.%X", tm_struct);

	return buf;
}

/*
* LogPrinter() - opens log file for writing at the end.
*/
LogPrinter::LogPrinter(std::string file_name)
{
	if (log_file.is_open()) {
		log_file.close();
	}

	log_file.open(file_name, std::ios::app);
}

/*
* ~LogPrinter() - closes log file.
*/
LogPrinter::~LogPrinter()
{
	log_file.close();
}

void LogPrinter::open(std::string file_name)
{
	if (log_file.is_open()) {
		log_file.close();
	}

	log_file.open(file_name);
}

void LogPrinter::print(std::string message)
{
	if (!log_file.is_open()) {
		return;
	}

	log_file << "[" << get_time() << "]::" << message << std::endl;
}

void LogPrinter::error(std::string error_message)
{
	if (log_file.is_open() != true) {
		return;
	}

	log_file << "[" << get_time() << "]::ERROR::" << error_message << std::endl;
}

void LogPrinter::close()
{
	log_file.close();
}
