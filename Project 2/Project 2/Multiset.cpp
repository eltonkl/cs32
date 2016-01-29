#include "Multiset.h"

Multiset::Multiset()
{
	_head = nullptr;
	_tail = nullptr;
	_size = 0;
}

//Helper function for copy constructor and assignment operator
void Multiset::_copyNodes(const Multiset& rhs)
{
	Node* rhsNode = rhs._head; //Current node in the rhs
	Node* lhsNode = nullptr; //Current node in the lhs

	if (rhsNode != nullptr) //Copy the first item, if it exists, and set up the linked list's head and tail
	{
		_head = new Node();
		_tail = _head;

		_head->data = rhsNode->data;

		lhsNode = _head;
		rhsNode = rhsNode->next;
	}

	while (rhsNode != nullptr) //For every other item in the linked list, if there are any
	{
		Node* previous = lhsNode;

		lhsNode = new Node();
		lhsNode->data = rhsNode->data;
		lhsNode->prev = previous;

		previous->next = lhsNode;
		_tail = lhsNode;
		rhsNode = rhsNode->next;
	}
}

//Helper function for destructor and assignment operator
void Multiset::_deleteNodes()
{
	while (_tail != nullptr)
	{
		Node* current = _tail;
		_tail = _tail->prev;
		delete current;
	}
	_head = nullptr;
}

//Copy constructor
Multiset::Multiset(const Multiset& rhs)
{
	_size = rhs._size;
	_copyNodes(rhs);
}

//Assignment operator
Multiset& Multiset::operator=(const Multiset& rhs)
{
	if (this == &rhs)
		return *this;
	//Check for aliasing

	_deleteNodes();
	_size = rhs._size;
	_copyNodes(rhs);

	return *this;
}

Multiset::~Multiset()
{
	_deleteNodes();
}

bool Multiset::empty() const
{
	return _size == 0;
}

int Multiset::size() const
{
	int size = 0;
	Node* i = _head;
	while (i != nullptr)
	{
		size += i->data.count;
		i = i->next;
	}
	return size;
}

int Multiset::uniqueSize() const
{
	return _size;
}

bool Multiset::insert(const ItemType& value)
{
	if (_head == nullptr) //For the first item
	{
		_head = new Node();
		_head->data.item = value;
		_head->data.count = 1;
		_tail = _head;
		_size = 1;
	}
	else //For everything after the first item
	{
		Node* i = _head;
		while (true)
		{
			if (i->data.item == value)
			{
				i->data.count++;
				break;
			}
			else if (i->next == nullptr) 
				//If this condition is met, then there is no item that already has the value being inserted
			{
				Node* next = new Node();
				next->data.item = value;
				next->data.count = 1;
				next->prev = i;

				i->next = next;
				_tail = next;

				_size++;

				break;
			}
			i = i->next;
		}
	}

	return true;
}

int Multiset::erase(const ItemType& value)
{
	Node* i = _head;
	while (i != nullptr)
	{
		if (i->data.item == value)
		{
			if (i->data.count > 1)
				//If the item count is greater than 1, can simply subtract one from the count
			{
				i->data.count--;
			}
			else
				//Otherwise, have to remove node from the linked list
			{
				Node* prev = i->prev;
				Node* next = i->next;
				if (prev != nullptr)
					prev->next = next;
				else
					//If prev is nullptr, then i was the head of the linked list
					_head = next;
				if (next != nullptr)
					next->prev = prev;
				else
					//If next is nullptr, then i was the tail of the linked list
					_tail = prev;

				delete i;
				_size--;
			}
			return 1;
		}
		i = i->next;
	}
	return 0;
}

int Multiset::eraseAll(const ItemType& value)
{
	int erased = 0;
	Node* i = _head;
	
	while (i != nullptr)
	{
		if (i->data.item == value)
		{
			erased = i->data.count;

			//Removing node from linked list
			Node* prev = i->prev;
			Node* next = i->next;
			if (prev != nullptr)
				prev->next = next;
			else
				//If prev is nullptr, then i was the head of the linked list
				_head = next;
			if (next != nullptr)
				next->prev = prev;
			else
				//If next is nullptr, then i was the tail of the linked list
				_tail = prev;

			delete i;
			_size--;
			break;
		}
		i = i->next;
	}

	return erased;
}

bool Multiset::contains(const ItemType& value) const
{
	Node* i = _head;

	while (i != nullptr)
	{
		if (i->data.item == value)
			return true;
		i = i->next;
	}

	return false;
}

int Multiset::count(const ItemType& value) const
{
	int count = 0;
	Node* i = _head;

	while (i != nullptr)
	{
		if (i->data.item == value)
		{
			count = i->data.count;
			break;
		}
		i = i->next;
	}

	return count;
}

int Multiset::get(int i, ItemType& value) const
{
	if (0 <= i && i < uniqueSize())
	{
		Node* current = _head;
		for (int j = 0; j < i; j++) //Make current point to the ith item in the linked list
		{
			current = current->next;
		}
		value = current->data.item;
		return current->data.count;
	}
	return 0;
}

void Multiset::swap(Multiset& other) //As the linked lists are allocated on the heap, can simply swap head and tail pointers between the two Multisets
{
	Node* temp;
	int size_temp;

	size_temp = _size;
	_size = other._size;
	other._size = size_temp;

	temp = _head;
	_head = other._head;
	other._head = temp;

	temp = _tail;
	_tail = other._tail;
	other._tail = temp;
}

Multiset::Node::Node() //To prevent any null pointer dereferences, here is a default constructor
{
	prev = nullptr;
	next = nullptr;
}

void combine(const Multiset& ms1, const Multiset& ms2, Multiset& result)
{
	bool addFirst = false; //addFirst indicates whether the contents of ms1 should be added to result
	bool addSecond = false; //addSecond does the same, but for ms2

	if (&ms1 == &result) //As result is ms1, only need to add ms2 to result to combine the two
		addSecond = true;
	else if (&ms2 == &result) //As result is ms2, only need to add ms1 to result to combine the two
		addFirst = true;
	else //Empty out result, as it may have contained something prior to this
	{
		int size = result.uniqueSize();
		for (int i = 0; i < size; i++)
		{
			ItemType item;
			result.get(0, item);
			result.eraseAll(item);
		}
		addFirst = true;
		addSecond = true;
	}

	if (addFirst) //ms1 and result are NOT the same object, so ms1.uniqueSize() remains the same for each iteration of the loop
	{
		for (int j = 0; j < ms1.uniqueSize(); j++)
		{
			ItemType item;
			int count = ms1.get(j, item);
			for (int k = 0; k < count; k++)
				result.insert(item);
		}
	}
	if (addSecond) //ms2 and result are NOT the same object, so ms2.uniqueSize() remains the same for each iteration of the loop
	{
		for (int j = 0; j < ms2.uniqueSize(); j++)
		{
			ItemType item;
			int count = ms2.get(j, item);
			for (int k = 0; k < count; k++)
				result.insert(item);
		}
	}
}

void subtract(const Multiset& ms1, const Multiset& ms2, Multiset& result)
{
	if (&ms2 == &result) //If ms2 and result are the same object, then modifications to result change ms2, so a copy of ms2 has to be made
	{
		Multiset temp = ms2;
		result = ms1;
		//temp and result are NOT the same object, so temp remains the same for each iteration of the loop
		for (int i = 0; i < temp.uniqueSize(); i++)
		{
			ItemType item;
			int count = temp.get(i, item);
			for (int j = 0; j < count; j++)
				result.erase(item);
		}
	}
	else //ms2 and result are NOT the same object, so ms2 remains the same for each iteration of the loop
	{
		result = ms1;
		//If ms1 and result are the same object, nothing happens
		for (int i = 0; i < ms2.uniqueSize(); i++)
		{
			ItemType item;
			int count = ms2.get(i, item);
			for (int j = 0; j < count; j++)
				result.erase(item);
		}
	}
}