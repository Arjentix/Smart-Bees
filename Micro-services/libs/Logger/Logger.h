/**
 * @author: Polyakov Daniil
 * @mail: arjentix@gmail.com
 * @github: Arjentix
 * @date: 19.10.2019
 */

#pragma once

/**
 * This file declares Logger class and object named logerr of type Logger,
 * which can be used for logging with simple interface like: logger << "Start";
 */

#include <fstream>
#include <string>

/**
 * Logger - class for logging your data into the log file provided in ctr.
 * You don't need to pass time at the beginning or std::endl at the end, cause
 * Logger do it automaticaly. For example the code below:
 * 		Logger my_logger("mylog.txt");
 * 		my_logger << "Start";
 * 	will write:
 * 		[YYYY-MM-DD.hh:mm:ss]::Start
 * with std::endl at the end into the mylog.txt
 */
class Logger {
private:
	std::ofstream _log_file;

	/*
	 * get_time() - returns string with current date and time.
	 */
	std::string _get_current_time();

public:
	/**
	 * Logger() - takes name of the log file as a parameter.
	 * All content of file will be discarded.
	 */
	Logger(const std::string& file_name);

	/**
	 * open() - opens another file and close previous.
	 */
	void open(const std::string& file_name);

	// Using cout-like interface
	template <typename T>
	Logger& operator<<(const T& obj)
	{
		_log_file << "[" << _get_current_time() << "]::" << obj << std::endl;
		return *this;
	}

	// Handling std::endl and etc.
	Logger& operator<<(std::ostream& (*f)(std::ostream&)) {
		f(_log_file);
		return *this;
	}
};

/**
 * logger - is used directly for logging. So it works pretty the same as cout.
 */
extern Logger logger;
