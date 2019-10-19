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
#include <sstream>
#include <mutex>

/**
 * Logger - class for logging your data into the log file provided in ctr.
 * You don't need to pass time at the beginning, cause
 * Logger do it automaticaly. For example the code below:
 * 		Logger my_logger("mylog.txt");
 * 		my_logger << "Start" << 5 << std::endl;
 * 	will write:
 * 		[YYYY-MM-DD.hh:mm:ss]::Start
 * into the mylog.txt
 */
class Logger {
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
		_locker.lock();
		_sstream << obj;
		return *this;
	}

	// Handling std::endl;
	Logger& operator<<(std::ostream& (*f)(std::ostream&)) {
		_log_file << "[" << _get_current_time() << "]::" << _sstream.str() << std::endl;
		_sstream = std::stringstream();

		_locker.unlock();
		return *this;
	}
private:
	std::ofstream _log_file;
	std::stringstream _sstream;
	std::mutex _locker;

	/*
	 * get_time() - returns string with current date and time.
	 */
	std::string _get_current_time();
};

/**
 * logger - is used directly for logging. So it works pretty the same as cout.
 */
extern Logger logger;
