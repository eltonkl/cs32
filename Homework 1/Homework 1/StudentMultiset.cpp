#include <iostream>

#include "StudentMultiset.h"

using namespace std;

StudentMultiset::StudentMultiset()
{
}

bool StudentMultiset::add(unsigned long id)
{
	return _set.insert(id);
}
// Add a student id to the StudentMultiset.  Return true if and only
// if the id was actually added.

int StudentMultiset::size() const
{
	return _set.size();
}
// Return the number of items in the StudentMultiset.  If an id was
// added n times, it contributes n to the size.

void StudentMultiset::print() const
{
	for (int i = 0; i < _set.uniqueSize(); i++)
	{
		unsigned long id = -1;
		_set.get(i, id);

		int count = _set.count(id);
		for (int j = 0; j < count; j++)
		{
			cout << id << endl;
		}
	}
}
// Print to cout every student id in the StudentMultiset one per line;
// print as many lines for each id as it occurs in the StudentMultiset.