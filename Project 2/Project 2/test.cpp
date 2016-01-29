#include <iostream>
#include <cassert>

#include "Multiset.h"

using namespace std;

void print(const Multiset& a)
{
	for (int i = 0; i < a.uniqueSize(); i++)
	{
		ItemType item;
		int count = a.get(i, item);
		for (int j = 0; j < count; j++)
			cout << item << " ";
	}
	cout << endl;
}

int main()
{
	auto equals = [](Multiset a, Multiset b) //Tests if two multisets have the same order and size
	{
		if (a.size() != b.size())
			return false;
		if (a.uniqueSize() != b.uniqueSize())
			return false;
		for (int i = 0; i < a.uniqueSize(); i++)
		{
			ItemType item1, item2;
			int count1 = a.get(i, item1);
			int count2 = b.get(i, item2);
			if (count1 != count2)
				return false;
			else if (item1 != item2)
				return false;
		}
		return true;
	};

	//Empty constructor
	Multiset test1;
	//Check for insert
	assert(test1.insert("a"));
	assert(test1.insert("a"));
	assert(test1.insert("b"));
	assert(test1.insert("c"));
	//Check for insert and size working correctly
	assert(test1.size() == 4);
	//Check for uniqueSize
	assert(test1.uniqueSize() == 3);

	//Check for copy constructor
	Multiset test2(test1);
	assert(equals(test1, test2));

	Multiset test3;
	//Check for assignment operator
	{
		Multiset temp(test2);
		test3 = temp;
	}
	assert(equals(test1, test2) && equals(test2, test3));

	//If assignment opreator and copy constructors don't function properly, these next two should fail because of dangling pointer dereferences or something else
	assert(test3.insert(""));
	assert(test3.contains(""));
	
	//Make sure erase functions work properly
	assert(test3.eraseAll("a") == 2);
	assert(test3.erase("b") == 1);
	assert(test3.eraseAll("c") == 1);
	assert(test3.erase("d") == 0);
	assert(test3.erase("") == 1);
	assert(test3.empty());
	//Any nullptr dereferences should be checked here, as the set is cleared and then inserted into
	assert(test3.insert("e"));
	assert(test3.size() == 1);
	assert(test3.uniqueSize() == 1);

	Multiset test_result;
	//Like addition:	x + x = y
	//					y - x = x
	//Additionally, verifying that nonempty multisets for the result parameter are properly handled
	combine(test1, test1, test_result);
	subtract(test_result, test1, test_result);
	assert(equals(test1, test_result));

	test_result = test3;
	//Aliasing checks
	combine(test1, test3, test3);
	subtract(test3, test1, test1);
	assert(equals(test1, test_result));
}
