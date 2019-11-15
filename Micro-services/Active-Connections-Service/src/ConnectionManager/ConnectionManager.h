/**
 * @author: Polyakov Daniil
 * @mail: arjentix@gmail.com
 * @github: Arjentix
 * @date: 17.10.2019
*/

#pragma once

#include "HTTPServer.h"
#include <unordered_map>
#include <mutex>
#include <future>

/**
 * ConnectionManager - manages connections with gates. It runs daemon server,
 * accepts connections from gates, receives their ids and stores.
 * Provides possibility to send message to the gate using it's id.
 */

class ConnectionManager {
public:
	ConnectionManager(int port);
	~ConnectionManager();

	/**
	 * send_command() -- sends command to the given gate.
	 * Can throw std::out_of_range exception if gate never was connected
	 */
	std::string send_command(
		const std::string& gate_id,
		const std::string& command
	) const;

private:
	HTTPServer _server;
	std::future<void> _server_future;
	std::unordered_map<std::string, int> _id_to_sock;
	mutable std::mutex _locker;

	void _connect_loop();
	void _get_and_store_id(int sock);
};
