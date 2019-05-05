#include "AliceConnector.h"
#include <iostream>
#include <exception>

using namespace std;

int main()
{
	try {
		AliceConnector	alice_conn("127.0.0.1", 4551);
		string 		token;
		for (int i = 0; i < 5; i++) {
			cout << "Waiting for token..." << endl;
			token = alice_conn.get_token();
			if (token != "") {
				cout << "Received token: " << token << endl;
				alice_conn.send_ok();
			}
		}

		return 0;
	}
	catch (const std::exception &error) {
		cout << error.what() << endl;
		return -1;
	}
}
