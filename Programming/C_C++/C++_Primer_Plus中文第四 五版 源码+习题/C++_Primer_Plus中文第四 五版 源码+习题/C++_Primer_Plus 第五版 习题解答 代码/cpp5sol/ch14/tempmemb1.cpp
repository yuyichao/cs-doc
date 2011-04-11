// tempmemb1.cpp -- template members
#include <iostream>
using std::cout;
using std::endl;

template <typename T>
class beta
{
private:
    template <typename V>  // declaration
    class hold;
    hold<T> q;             
    hold<int> n;          
public:
    beta( T t, int i) : q(t), n(i) {}
    template<typename U>   // declaration
    U blab(U u, T t);
    void Show() const { q.show(); n.show();}
};

// member definition
template <typename T>
  template<typename V>
    class beta<T>::hold
    {
    private:
        V val;
    public:
        hold(V v  = 0) : val(v) {}
        void show() const { cout << val << endl; }
        V Value() const { return val; }
    };

// member definition
template <typename T>
  template <typename U>
    U beta<T>::blab(U u, T t)
    { 
       return (n.Value() + q.Value()) * u / t;
    }

int main()
{
    beta<double> guy(3.5, 3);

    guy.Show();
    cout << guy.blab(10, 2.3) << endl;
    cout << "Done\n";
    return 0; 
}
