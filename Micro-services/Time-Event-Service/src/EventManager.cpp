/**
 * @author: Polyakov Daniil
 * @mail: arjentix@gmail.com
 * @github: Arjentix
 * @date: 09.10.2019
*/

#include "EventManager.h"
#include <thread>

using namespace std;

Event::Event(const string& time_str, const string& com)
	: command(com)
{
	strptime(time_str.c_str(), "", &time_stamp);	// TODO format
}

void to_json(nlohmann::json& j, const Event& event)
{
	char buffer[512];
	strftime(buffer, 512, "", &event.time_stamp);	// TODO format
	j = {{"time_stamp", buffer}, {"command", event.command}};
}

void from_json(const nlohmann::json& j, Event& event)
{
	string time_str = j.at("time_stamp").get<std::string>();
	strptime(time_str.c_str(), "", &event.time_stamp);	// TODO format

	j.at("command").get_to(event.command);
}



void EventManager::add(const string& id, const Event& event)
{
	lock_guard guard(_locker);

	_id_to_events[id].insert(event);
	size_t event_pos = _id_to_events.size() - 1;
	_futures.push_back(async(
		_wait_and_send, this, id, event, event_pos
	));
}

const set<Event>& EventManager::get_events_for(const string& id) const
{
	if (_id_to_events.count(id)) {
		return _id_to_events.at(id);
	}

	throw invalid_argument("No such id");
}

void EventManager::remove(const string& id, size_t num)
{
	lock_guard guard(_locker);
	if (_id_to_events.count(id)) {
		if (num < _id_to_events[id].size()) {
			set<Event>& cur_events = _id_to_events[id];
			cur_events.erase(next(cur_events.begin(), num));
			return;
		}

		throw invalid_argument("num is bigger than events count");
	}

	throw invalid_argument("No such id");
}

void EventManager::_wait_and_send(
	const string& id,
	const Event& event, 
	size_t event_pos
)
{
	// Sleeping for difference between given time and current time
	tm time_info = event.time_stamp;
	this_thread::sleep_for(
		chrono::seconds(
			static_cast<int>(difftime(mktime(&time_info), time(0)))
		)
	);

	// TODO sending command

	remove(id, event_pos);
}
