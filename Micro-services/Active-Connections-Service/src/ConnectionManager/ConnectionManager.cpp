/**
 * @author: Polyakov Daniil
 * @mail: arjentix@gmail.com
 * @github: Arjentix
 * @date: 17.10.2019
*/

#include "ConnectionManager.h"
#include "Logger.h"
#include <vector>

using namespace std;

ConnectionManager::ConnectionManager()
{
	_server.start_server(4551);
	_server.turn_to_listen(5);

	_server_future = async(&_connect_loop, *this);
}

ConnectionManager::~ConnectionManager()
{
	for (const auto& [id, sock] : _id_to_sock) {
		HTTPServer::close_con(sock);
	}
}

void ConnectionManager::send_command(
	const string& id,
	const string& command
) const
{
	HTTPServer::send_raw(_id_to_sock.at(id), command);
}

void ConnectionManager::_connect_loop()
{
	vector<future<void>> client_futures;
	for(;;) {
		int gate_sock = _server.connect_client();
		logger << "Gate connected on socket " << gate_sock;
		client_futures.push_back(async(&_get_and_store_id, *this, gate_sock));
	}
}

void ConnectionManager::_get_and_store_id(int sock)
{
	string id = HTTPServer::get_raw(sock);

	lock_guard guard(_locker);
	_id_to_sock[id] = sock;
}
