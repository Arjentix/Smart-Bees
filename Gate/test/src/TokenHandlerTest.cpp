#include "TokenHandler.h"
#include <string>
#include <iostream>
#include <fstream>

using namespace std;

int main()
{
	ofstream base("tokens.base", ios_base::out | ios_base::trunc);
	base << "Лампа : smart_home/lamp : command 1" << endl;
	base << "Чайник : smart_home/teapot : command 1" << endl;
	base << "Чайник 80 градусов : smart_home/teapot : command 2" << endl;
	base.close();

	TokenHandler	tok_hand("tokens.base");
	string		topic;
	string		command;
	bool		res;

	res = tok_hand.find("Чайник", topic, command);
	if (res == true) {
		cout << topic << endl;
		cout << command << endl;
		cout << endl;
	}
	else {
		cout << "No results" << endl;
		cout << endl;
	}

	res = tok_hand.find("Телевизор", topic, command);
	if (res == true) {
		cout << topic << endl;
		cout << command << endl;
		cout << endl;
	}
	else {
		cout << "No results" << endl;
		cout << endl;
	}

	res = tok_hand.find("Лампа", topic, command);
	if (res == true) {
		cout << topic << endl;
		cout << command << endl;
		cout << endl;
	}
	else {
		cout << "No results" << endl;
		cout << endl;
	}

	res = tok_hand.find("Чайник 80 градусов", topic, command);
	if (res == true) {
		cout << topic << endl;
		cout << command << endl;
		cout << endl;
	}
	else {
		cout << "No results" << endl;
		cout << endl;
	}

	return 0;
}
