#include <iostream>
#include <time.h>
using namespace std;

//#define _IMMUTABLE_

#if defined (_IMMUTABLE_)
	#include "ImmutableString.h"
	typedef ImmutableString String;
#else
	#include <string>
	typedef string String;
#endif

const long Times = 500000;

void main()
{

	#if defined (_IMMUTABLE_)
		cout<<"Using Immutable String!"<<endl;
	#else
		cout<<"Using Mutable String!"<<endl;
	#endif

	void DisplayTime();

	DisplayTime();

	String str = String("This is a string!");
	for(int i=0; i<Times; i++)
	{
		String * pstr = new String("This is another string!");
		str = *pstr;
		delete pstr;
	}

	DisplayTime();
}

void DisplayTime()
{
	time_t tt = time(&tt);
	cout<<tt<<endl;
}