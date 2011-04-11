// sales.cpp -- Sales implementation
#include "sales.h"

Sales::bad_index::bad_index(int ix, const char * s )
    : std::logic_error(s), bi(ix)
{
}

Sales::Sales(int yy)
{
    year = yy;
    for (int i = 0; i < MONTHS; ++i)
        gross[i] = 0;
}

Sales::Sales(int yy, const double * gr, int n)
{
    year = yy;
    int lim = (n < MONTHS)? n : MONTHS;
    int i;
    for (i = 0; i < lim; ++i)
        gross[i] = gr[i];
    // for i > n and i < MONTHS
    for ( ; i < MONTHS; ++i)
        gross[i] = 0;
}

double Sales::operator[](int i) const throw(std::logic_error)
{
    if(i < 0 || i >= MONTHS)
        throw bad_index(i);
    return gross[i];
}

double & Sales::operator[](int i) throw(std::logic_error)
{
    if(i < 0 || i >= MONTHS)
        throw bad_index(i);
    return gross[i];
}

LabeledSales::nbad_index::nbad_index(const char * lb, int ix,
           const char * s ) : Sales::bad_index(ix, s)
{
    std::strcpy(lbl, lb);
}

LabeledSales::LabeledSales(const char * lb, int yy)
         : Sales(yy)
{
    std::strcpy(label, lb);
}

LabeledSales::LabeledSales(const char * lb, int yy, const double * gr, int n)
        : Sales(yy, gr, n)
{
    std::strcpy(label, lb);
}

double LabeledSales::operator[](int i) const throw(std::logic_error)
{
    if(i < 0 || i >= MONTHS)
        throw nbad_index(Label(), i);
    return Sales::operator[](i);
}

double & LabeledSales::operator[](int i) throw(std::logic_error)
{
    if(i < 0 || i >= MONTHS)
        throw nbad_index(Label(), i);
    return Sales::operator[](i);
}
