// enum.cpp -- a simple enumeration
#include <iostream>
#include <cstring>
using namespace	std;
enum spectrum {red, orange, yellow, green, blue, violet, indigo, ultraviolet};

int	main()
{
	enum spectrum a, b,c;
	int d;
	a = orange;
	b = spectrum(5);
	d = orange + red;
	cout << d << endl;
	return 0;
}