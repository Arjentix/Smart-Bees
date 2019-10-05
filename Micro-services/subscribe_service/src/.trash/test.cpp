#include <iostream>
#include <iomanip>
#include <sstream>
#include <ctime>

using namespace std;

int main() {
	time_t t = time(NULL);
	struct tm *tm = localtime(&t);
	char c[80];
	strftime(c, 80, "%F %T", tm);

	string s = string(c);
	cout << s << endl;

}
