//#include "database.h"
#include "server.h"


using namespace std;

const char* DB_HOSTNAME = "localhost";
const char* DB_NAME = "alice_subs";
const char* DB_USERNAME = "http_server";
const char* DB_PASSWORD = "12345678";



int main()
{
	/*
	try {
        DataBase db;
		db.init(DB_HOSTNAME, DB_NAME, DB_USERNAME, DB_PASSWORD);
        bool bRet = db.exec();
        if (!bRet) cout << "ERROR!" << endl;
    } catch (char *e) {
        cerr << "[EXCEPTION] " << e << endl;
        return 1;
    }
    return 0;
*/


	int port = 3000;

	Server server;
	server.start_server(port);
	server.turn_to_listen(100);
	while(true) {
		server.connect_client();
	}
}
