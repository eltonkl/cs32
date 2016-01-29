#include "Multiset.h"  // class template from problem 1
#include <string>
using namespace std;

class URL
{
public:
	URL(string i) : m_id(i) {}
	URL() : m_id("http://cs.ucla.edu") {}
	string id() const { return m_id; }
private:
	string m_id;
};

int main()
{
	Multiset<int> mi;
	mi.insert(7);  // OK
	Multiset<string> ms;
	ms.insert("http://www.symantec.com");  // OK
	Multiset<URL> mu;
	mu.insert(URL("http://www.symantec.com"));  // error!
}