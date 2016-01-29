#include <string>

#include "newMultiset.h"

Multiset::Multiset() :
_max(DEFAULT_MAX_ITEMS)
{
	_size = 0;
	_items = new Item[_max];
}

Multiset::Multiset(int size) :
_max(size)
{
	_size = 0;
	_items = new Item[_max];
}

Multiset::~Multiset()
{
	delete[] _items;
}

Multiset::Multiset(const Multiset& rhs) :
_max(rhs._max)
{
	_items = new Item[_max];
	for (int i = 0; i < _max; i++)
		_items[i] = rhs._items[i];
	_size = rhs._size;
}

Multiset& Multiset::operator=(const Multiset& rhs)
{
	if (this == &rhs)
		return *this;

	if (_max != rhs._max)
	{
		_max = rhs._max;
		delete[] _items;
		_items = new Item[rhs._max];
	}
	for (int i = 0; i < _max; i++)
		_items[i] = rhs._items[i];

	_size = rhs._size;

	return *this;
}

bool Multiset::empty() const
{
	return _size == 0;
}

int Multiset::size() const
{
	int size = 0;
	for (int i = 0; i < _size; i++)
	{
		size += _items[i].count;
	}
	return size;
}

int Multiset::uniqueSize() const
{
	return _size;
}

bool Multiset::insert(const ItemType& value)
{
	int index;
	for (index = 0; index < _size; index++)
	{
		if (_items[index].item == value)
		{
			_items[index].count++;
			return true;
		}
	}
	if (index < _max)
	{
		_items[index].item = value;
		_items[index].count = 1;
		_size++;
		return true;
	}
	else
		return false;
}

int Multiset::erase(const ItemType& value)
{
	for (int i = 0; i < _size; i++)
	{
		if (_items[i].item == value)
		{
			if (_items[i].count > 1)
				_items[i].count--;
			else
			{
				for (int j = i; j < _size - 1; j++)
					_items[j] = _items[j + 1];
				_size--;
			}
			return 1;
		}
	}

	return 0;
}

int Multiset::eraseAll(const ItemType& value)
{
	int erased = 0;
	for (int i = 0; i < _size; i++)
	{
		if (_items[i].item == value)
		{
			erased = _items[i].count;
			for (int j = i; j < _size - 1; j++)
			{
				_items[j] = _items[j + 1];
			}
			_size--;
		}
	}

	return erased;
}

bool Multiset::contains(const ItemType& value) const
{
	for (int i = 0; i < _size; i++)
	{
		if (_items[i].item == value)
			return true;
	}
	return false;
}

int Multiset::count(const ItemType& value) const
{
	int count = 0;
	for (int i = 0; i < _size; i++)
	{
		if (_items[i].item == value)
			count = _items[i].count;
	}
	return count;
}

int Multiset::get(int i, ItemType& value) const
{
	if (0 <= i && i < uniqueSize())
	{
		value = _items[i].item;
		return _items[i].count;
	}

	return 0;
}

void Multiset::swap(Multiset& other)
{
	Item* items_temp = _items;
	int size_temp = _size;
	int max_temp = _max;

	_items = other._items;
	_size = other._size;
	_max = other._max;

	other._items = items_temp;
	other._size = size_temp;
	other._max = max_temp;
}