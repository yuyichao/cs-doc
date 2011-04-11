// stocks.cpp -- the whole program
#include <iostream>
#include <cstring>  

class Stock  // class declaration
{
private:
    char company[30];
    int shares;
    double share_val;
    double total_val;
    void set_tot() { total_val = shares * share_val; }
public:
    void acquire(const char * co, int n, double pr);
    void buy(int num, double price);
    void sell(int num, double price);
    void update(double price);
    void show();
};    // note semicolon at the end

void Stock::acquire(const char * co, int n, double pr)
{
    std::strncpy(company, co, 29);  // truncate co to fit company
    company[29] = '\0';
    if (n < 0)
    {
        std::cerr << "Number of shares can't be negative; "
                  << company << " shares set to 0.\n";
        shares = 0;
    }
    else
        shares = n;
    share_val = pr;
    set_tot();
}

void Stock::buy(int num, double price)
{
     if (num < 0)
    {
        std::cerr << "Number of shares purchased can't be negative. "
             << "Transaction is aborted.\n";
    }
    else
    {
        shares += num;
        share_val = price;
        set_tot();
    }
}

void Stock::sell(int num, double price)
{
    using std::cerr;
    if (num < 0)
    {
        cerr << "Number of shares sold can't be negative. "
             << "Transaction is aborted.\n";
    }
    else if (num > shares)
    {
        cerr << "You can't sell more than you have! "
             << "Transaction is aborted.\n";
    }
    else
    {
        shares -= num;
        share_val = price;
        set_tot();
    }
}

void Stock::update(double price)
{
    share_val = price;
    set_tot();
}

void Stock::show()
{
    using std::cout;
    using std::endl;
    cout << "Company: " << company
        << "  Shares: " << shares << endl
        << "  Share Price: $" << share_val
        << "  Total Worth: $" << total_val << endl; 
}

int main()
{
    using std::cout;
    using std::ios_base;

    Stock stock1;
    stock1.acquire("NanoSmart", 20, 12.50);
    cout.setf(ios_base::fixed);        // #.## format
    cout.precision(2);                 // #.## format
    cout.setf(ios_base::showpoint);    // #.## format
    stock1.show();
    stock1.buy(15, 18.25);
    stock1.show();
    stock1.sell(400, 20.00);
    stock1.show();
    return 0;
}
