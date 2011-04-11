#include <iostream>
using namespace std;

struct Data
{
    double data[200];
};

struct Junk
{
    int junk[100];
};

int main()
{
	Data d = {2.5e33, 3.5e-19, 20.2e32};
	char * pch = (char *) (&d);   // typecast #1 – convert to string
	char ch = long (&d);          // typecast #2 - convert address to a char
	Junk * pj = (Junk *) (&d);    // typecast #3 - convert to Junk pointer
	cout << pch << ": pch\n";
	cout << ch << ": ch\n";
	for (int i = 0; i < 6; ++i)
		cout << pj->junk[i] << endl;

	return 0;
}