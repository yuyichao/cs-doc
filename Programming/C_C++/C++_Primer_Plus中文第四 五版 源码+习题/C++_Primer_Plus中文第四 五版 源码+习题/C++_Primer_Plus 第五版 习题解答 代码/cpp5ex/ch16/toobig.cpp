#include <iostream>
#include <list>
#include <iterator>

template<class T>
class TooBig
{
private:
    T cutoff;
public:
    TooBig(const T & t) : cutoff(t) {}
    bool operator()(const T & v) { return v > cutoff; }
};
int main()
{
	using std::list;
	using std::cout;
	using std::endl;
	TooBig<int> f100(100);
	list<int> froobies;
	list<int> scores;
	int v[10] = {50, 120, 90, 180, 60, 210, 415, 88, 188, 201};
	froobies.insert(froobies.begin(), v, v + 10);
	scores.insert(scores.begin(), v, v + 10);
	std::ostream_iterator<int, char> out(cout, " ");
	copy(froobies.begin(), froobies.end(), out);
	cout << endl;
	copy(scores.begin(), scores.end(), out);
	cout << endl;
	froobies.remove_if(f100);               // use a named function object
	scores.remove_if(TooBig<int>(200));     // construct a function object
	copy(froobies.begin(), froobies.end(), out);
	cout << endl;
	copy(scores.begin(), scores.end(), out);
	cout << endl;
	return 0;
}
