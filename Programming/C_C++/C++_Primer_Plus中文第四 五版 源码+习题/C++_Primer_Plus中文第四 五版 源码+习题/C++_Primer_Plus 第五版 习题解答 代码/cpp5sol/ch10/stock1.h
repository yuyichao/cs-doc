// stock1.h – Stock class declaration with constructors, destructor added
#ifndef STOCK1_H_
#define STOCK1_H_

class Stock
{
private:
    char company[30];
    int shares;
    double share_val;
    double total_val;
    void set_tot() { total_val = shares * share_val; }
public:
    Stock();        // default constructor
    Stock(const char * co, int n = 0, double pr = 0.0);
    ~Stock();       // noisy destructor
    void buy(int num, double price);
    void sell(int num, double price);
    void update(double price);
    void show();
};

#endif
