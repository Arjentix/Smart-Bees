#include "PhotonConfigReader.h"
#include <string>
#include <iostream>

using namespace std;

int main()
{
	ofstream config("photon.conf", ios_base::out | ios_base::trunc);
	config << "40:a3:cc:ff:3b:d3 = smart_home/lamp" << endl;
	config << "50:a3:ds:fk:3b:d3 = smart_home/teapot" << endl;
	config.close();

	PhotonConfigReader	photon_conf_reader("photon.conf");
	string			mac;
	string 			topic;
	bool			res;

	res = photon_conf_reader.get_next(mac, topic);
	while (res != false) {
		cout << mac << endl;
		cout << topic << endl;
		cout << endl;

		res = photon_conf_reader.get_next(mac, topic);
	}

	return 0;
}
