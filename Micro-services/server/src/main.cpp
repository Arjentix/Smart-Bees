#include "server.h"

using namespace std;

int main()
{
	int port = 3000;


	Server server;
	server.start_server(port);
	while(1) {
		server.wait_for_con();
	}
}
