// cinexcp.cpp
#include <iostream>
#include <exception>
using namespace std;

int main()
{
    // have failbit cause an exception to be thown
    cin.exceptions(ios_base::failbit);
    cout.precision(2);
    cout << showpoint << fixed;
    cout << "Enter numbers: ";
    double sum = 0.0;
    double input;
    try {
	    while (cin >> input)
	    {
	        sum += input;
	    }
    } catch(ios_base::failure & bf)
    {
        cout << bf.what() << endl;
        cout << "O! the horror!\n";
    }

    cout << "Last value entered = " << input << "\n";
    cout << "Sum = " << sum << "\n";
    return 0; 
}
