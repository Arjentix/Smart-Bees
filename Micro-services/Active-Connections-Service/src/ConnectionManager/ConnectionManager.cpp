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

ConnectionManager::ConnectionManager(int port)
{
	_server.start_server(port);
	_server.turn_to_listen(5);

	_server_future = async(&ConnectionManager::_connect_loop, this);
}

ConnectionManager::~ConnectionManager()
{
	for (const auto& [id, sock] : _id_to_sock) {
		HTTPServer::close_con(sock);
	}
}

string ConnectionManager::send_command(
	const string& id,
	const string& command
) const
{
	int sock = _id_to_sock.at(id);

	HTTPServer::send_raw(sock, "Check");
	logger << "Sended Check" << endl;
	HTTPServer::get_raw(sock);
	logger << "Check passed" << endl;
	HTTPServer::send_raw(sock, command);
	logger << "Sended command" << endl;

	return HTTPServer::get_raw(sock);
}

void ConnectionManager::_connect_loop()
{
	logger << "Loop for gates connections started" << endl;
	vector<future<void>> client_futures;
	for(;;) {
		int gate_sock = _server.connect_client();
		logger << "Gate connected on socket " << gate_sock << endl;
		client_futures.push_back(async(
			&ConnectionManager::_get_and_store_id,
			this,
			gate_sock
		));
	}
}

void ConnectionManager::_get_and_store_id(int sock)
{
	string id = HTTPServer::get_raw(sock);
	logger << "Got gate id: " << id << endl;

	lock_guard guard(_locker);
	if (_id_to_sock.count(id)) {
		HTTPServer::close_con(_id_to_sock.at(id));
	}
	_id_to_sock[id] = sock;
}
