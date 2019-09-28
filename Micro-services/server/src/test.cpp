#include<iostream>
#include<vector>

using namespace std;

vector<int*> v = {0, 0, 0};

int* f(int i) {
	for(auto j: v)
		if(j == 0) {
			return j;
		}
}
int main() {
	int* tmp = f(0);
	&tmp = 5;

	for(auto j: v)
		cout << j << " ";
	cout << endl;
}
