// constcast.cpp -- using const_cast<>
#include <iostream>
using std::cout;
using std::endl;

void change(const int * pt, int n);

int main()
{
    int pop1 = 38383;
    const int pop2 = 2000;
    
    cout << "pop1, pop2: " << pop1 << ", " << pop2 << endl;
    change(&pop1, -1);
    change(&pop2, -1);
    cout << "pop1, pop2: " << pop1 << ", " << pop2 << endl;

    return 0;
}

void change(const int * pt, int n)
{
    int * pc;
    if (n < 0)
    {
        pc = const_cast<int *>(pt);
        *pc = 100;
    }
}
