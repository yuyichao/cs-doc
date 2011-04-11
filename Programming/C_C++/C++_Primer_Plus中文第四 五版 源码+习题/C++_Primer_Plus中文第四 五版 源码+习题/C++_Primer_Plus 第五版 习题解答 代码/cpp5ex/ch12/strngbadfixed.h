// strngbad.h -- flawed string class definition
#include <iostream>
using std::ostream;
#ifndef STRNGBAD_H_
#define STRNGBAD_H_
class StringBad
{
private:
    char * str;                // pointer to string
    int len;                   // length of string
    static int num_strings;    // number of objects
public:
    StringBad(const char * s); // constructor
    StringBad();               // default constructor
    StringBad(const StringBad &); // copy constructor
    ~StringBad();              // destructor
    StringBad & operator=(const StringBad &);
    // friend function
    friend ostream & operator<<(ostream & os, 
                       const StringBad & st);
};
#endif
