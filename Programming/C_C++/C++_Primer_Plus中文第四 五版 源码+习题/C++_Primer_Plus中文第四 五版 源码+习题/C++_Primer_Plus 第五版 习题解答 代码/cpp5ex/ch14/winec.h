// winec.h -- wine class using containment
#ifndef WINEC_H_
#define WINEC_H_

#include <iostream>
#include <string>
#include <valarray>
#include "pairs.h"

class Wine
{
private:
    typedef std::valarray<int> ArrayInt;
	typedef Pair<ArrayInt, ArrayInt> PairArray;
    std::string label;      // wine brandname
    int years;              // number of years
    PairArray data;
    
public:
    Wine() : label("none"), years(0), data(ArrayInt(),ArrayInt()) {}
    Wine(const char * l, int y, const int yr[], const int bot[]);
    Wine(const char * l, const ArrayInt & yr, const ArrayInt & bot);
    Wine(const char * l, const PairArray & yr_bot);
    Wine(const char * l, int y);
    void GetBottles();
    void Show() const;
	const std::string & Label() { return label; }
	int sum() const { return data.second().sum(); }
};

#endif
