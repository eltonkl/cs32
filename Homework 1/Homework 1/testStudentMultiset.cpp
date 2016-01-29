#include "StudentMultiset.h"

#include <iostream>
#include <cassert>

int main()
{
	StudentMultiset sm;
	sm.add(50505050);
	sm.add(550251);
	sm.add(50505050);
	sm.add(50);
	sm.add(5);
	sm.add(6);
	sm.add(1);
	sm.print();
	assert(sm.size() == 7);

	return 0;
}