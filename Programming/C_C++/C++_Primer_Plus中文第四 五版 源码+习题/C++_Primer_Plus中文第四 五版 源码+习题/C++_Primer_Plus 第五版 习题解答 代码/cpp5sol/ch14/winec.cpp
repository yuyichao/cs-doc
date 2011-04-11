// winec.cpp  -- Wine class with containment
#include <iostream>
#include "winec.h"

using std::cin;
using std::cout;
using std::cerr;
using std::endl;


Wine::Wine(const char * l, int y, const int yr[], const int bot[])
  : label(l), years(y), data(ArrayInt(yr,y),ArrayInt(bot,y) )
{
}

Wine::Wine(const char * l, const ArrayInt & yr, const ArrayInt & bot)
  : label(l), years(yr.size()), data(ArrayInt(yr), ArrayInt(yr))
{
	if (yr.size() != bot.size())
	{
		cerr << "Year data, bottle data mismatch, array set to 0 size.\n";
		years = 0;
        data = PairArray(ArrayInt(),ArrayInt());
	}
	else
	{
		data.first() = yr;
		data.second() = bot;
	}
}
Wine::Wine(const char * l, const PairArray & yr_bot)
: label(l), years(yr_bot.first().size()), data(yr_bot)  { }


Wine::Wine(const char * l, int y) : label(l), years(y),
	data(ArrayInt(0,y),ArrayInt(0,y))
{}

void Wine::GetBottles()
{
	if (years < 1)
	{
		cout << "No space allocated for data\n";
		return;
	}

	cout << "Enter " << label <<
			" data for " << years << " year(s):\n";
	for (int i = 0; i < years; i++)
	{
		cout << "Enter year: ";
		cin >> data.first()[i];
		cout << "Enter bottles for that year: ";
		cin >> data.second()[i];
	}
}


void Wine::Show() const
{
	cout << "Wine: " << label << endl;
	cout << "\tYear\tBottles\n";
	for (int i = 0; i < years; i++)
		cout << '\t' << data.first()[i] 
		     << '\t' << data.second()[i] << endl;
}
