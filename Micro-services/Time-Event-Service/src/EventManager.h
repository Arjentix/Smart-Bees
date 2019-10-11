/**
 * @author: Polyakov Daniil
 * @mail: arjentix@gmail.com
 * @github: Arjentix
 * @date: 09.10.2019
*/

#pragma once

#include <nlohmann/json.hpp>
#include <string>
#include <map>
#include <vector>
#include <set>
#include <ctime>
#include <future>
#include <mutex>

struct Event {
	Event(const std::string& time_str, const std::string& com);

	std::tm time_stamp;
	std::string command;
};

void to_json(nlohmann::json& j, const Event& event);

void from_json(const nlohmann::json& j, Event& event);

class EventManager {
public:
	void add(const std::string& id, const Event& event) noexcept;
	const std::set<Event>& get_events_for(const std::string& id) const throw(invalid_argument);
	void remove(const std::string& id, size_t num) throw(invalid_argument);

private:
	std::map<std::string, std::set<Event>> _id_to_events;
	std::vector<std::future<void>> _futures;
	std::mutex _locker;
	void _wait_and_send(const string& id, const Event& event, size_t event_pos);
};
