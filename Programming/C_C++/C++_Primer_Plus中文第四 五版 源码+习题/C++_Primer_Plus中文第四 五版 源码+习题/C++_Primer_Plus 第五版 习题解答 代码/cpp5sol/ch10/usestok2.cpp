// usestok2.cpp -- use the Stock class
// compile with stock2.cpp
#include <iostream>
#include "stock2.h"

const int STKS = 4;
int main()
{
    using std::cout;
    using std::ios_base;

// create an array of initialized objects
    Stock stocks[STKS] = {
        Stock("NanoSmart", 12, 20.0),
        Stock("Boffo Objects", 200, 2.0),
        Stock("Monolithic Obelisks", 130, 3.25),
        Stock("Fleep Enterprises", 60, 6.5)
        };

    cout.precision(2);                               // #.## format
    cout.setf(ios_base::fixed, ios_base::floatfield);// #.## format
    cout.setf(ios_base::showpoint);                  // #.## format

    cout << "Stock holdings:\n";
    int st;
    for (st = 0; st < STKS; st++)
        stocks[st].show();

    Stock top = stocks[0];
    for (st = 1; st < STKS; st++)
        top = top.topval(stocks[st]);
    cout << "\nMost valuable holding:\n";
    top.show();

    return 0; 
}
