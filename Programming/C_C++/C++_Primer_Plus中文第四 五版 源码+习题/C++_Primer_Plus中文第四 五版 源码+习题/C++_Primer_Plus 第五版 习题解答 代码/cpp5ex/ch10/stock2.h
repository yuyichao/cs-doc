// stock2.h -- augmented version
#ifndef STOCK2_H_
#define STOCK2_H_

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
    ~Stock();       // do-nothing destructor
    void buy(int num, double price);
    void sell(int num, double price);
    void update(double price);
    void show()const;
    const Stock & topval(const Stock & s) const;
};

#endif
