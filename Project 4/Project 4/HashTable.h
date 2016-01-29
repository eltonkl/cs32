#ifndef HASHTABLE_H
#define HASHTABLE_H

unsigned int computeHash(int key);

template <typename KeyType, typename ValueType>
class HashTable
{
public:
	HashTable(unsigned int numBuckets, unsigned int capacity);
	~HashTable();

	bool isFull() const;
	bool set(const KeyType& key, const ValueType& value, bool permanent = false);
	bool get(const KeyType& key, ValueType& value) const;
	bool touch(const KeyType& key);
	bool discard(KeyType& key, ValueType& value);
private:
	// We prevent a HashTable from being copied or assigned by declaring the 
	// copy constructor and assignment operator private and not implementing them. 
	HashTable(const HashTable&);
	HashTable& operator=(const HashTable&);

	//Each node is in two linked lists: the LinkedList class, and the history linked list which is managed by the HashTable
	struct Node
	{
		//LinkedList only interacts with these 5 data members
		Node* prev;
		Node* next;
		KeyType key;
		ValueType value;
		bool permanent;
		//HashTable manages the history of the whole HashTable using these two pointers
		Node* hist_before;
		Node* hist_later;
	};

	//Each LinkedList represents a bucket in this HashTable implementation
	class LinkedList
	{
	public:
		LinkedList();
		~LinkedList();

		Node* insert(const KeyType& key, const ValueType& value, bool permanent);
		//void erase(const KeyType& key);
		//bool empty() const;
		Node* get(const KeyType& key) const;
	private:
		Node* _head;
		Node* _last;
	};

	/*
	The HashTable could look something like this:	0: ->(0, 25, false)<->(2, 2, true)<->(4, 6, false)<-
													1: ->(1, 16, false)<->(3, 26234, false)<-
	_hist_first_modified signifies the start of the history circular doubly-linked list, which could look like this:
	->(4, 6, false)<->(3, 26234, false)<->(0, 25, false)<->(1, 16, false)<-
	_hist_last_modified points to (1, 16, false), which is the most recently modified Node.
	*/

	unsigned int getBucketForKey(const KeyType& key) const;
	bool touchNode(Node* node);
	void deleteNode(Node* node);

	unsigned int _numBuckets;
	unsigned int _currentCapacity;
	unsigned int _maxCapacity;
	LinkedList* _items;
	Node* _hist_first_modified;
	Node* _hist_last_modified;
};

template <typename KeyType, typename ValueType>
HashTable<KeyType, ValueType>::HashTable(unsigned int numBuckets, unsigned int capacity) :
_numBuckets(numBuckets), _maxCapacity(capacity)
{
	_currentCapacity = 0;
	_hist_first_modified = nullptr;
	_hist_last_modified = nullptr;
	_items = new LinkedList[numBuckets];
}

template <typename KeyType, typename ValueType>
HashTable<KeyType, ValueType>::~HashTable()
{
	delete[] _items;
}

template <typename KeyType, typename ValueType>
bool HashTable<KeyType, ValueType>::isFull() const
{
	return _currentCapacity == _maxCapacity;
}

template <typename KeyType, typename ValueType>
bool HashTable<KeyType, ValueType>::set(const KeyType& key, const ValueType& value, bool permanent)
{
	unsigned int bucket = getBucketForKey(key);
	Node* current = _items[bucket].get(key);
	if (current == nullptr) //Key doesn't exist in hashtable
	{
		if (isFull())
			return false;
		else
		{
			current = _items[bucket].insert(key, value, permanent);
			if (!permanent)
			{
				if (_hist_first_modified == nullptr)
				{
					_hist_first_modified = current;
					_hist_last_modified = current;
				}
				//Insert node into history linked list
				current->hist_later = _hist_first_modified;
				current->hist_before = _hist_last_modified;
				_hist_last_modified->hist_later = current;
				_hist_last_modified = current;
				_hist_first_modified->hist_before = current;
			}
			_currentCapacity++;
			return true;
		}
	}
	else //Key already exists in hashtable, update value
	{
		current->value = value;
		touchNode(current);
		return true;
	}
}

template <typename KeyType, typename ValueType>
bool HashTable<KeyType, ValueType>::get(const KeyType& key, ValueType& value) const
{
	unsigned int bucket = getBucketForKey(key);
	Node* current = _items[bucket].get(key);
	if (current == nullptr)
		return false;
	else
	{
		value = current->value;
		return true;
	}
}

template <typename KeyType, typename ValueType>
bool HashTable<KeyType, ValueType>::touch(const KeyType& key)
{
	unsigned int bucket = getBucketForKey(key);
	Node* current = _items[bucket].get(key);
	return touchNode(current);
}

template <typename KeyType, typename ValueType>
bool HashTable<KeyType, ValueType>::discard(KeyType& key, ValueType& value)
{
	Node* current = _hist_first_modified;
	if (current == nullptr) //No non-permanent entries in the HashTable
		return false;
	else
	{
		key = current->key;
		value = current->value;

		if (_hist_first_modified == _hist_last_modified)
		{
			_hist_first_modified = nullptr;
			_hist_last_modified = nullptr;
		}
		else
		{
			_hist_first_modified = current->hist_later;
			_hist_first_modified->hist_before = _hist_last_modified;
			_hist_last_modified->hist_later = _hist_first_modified;
		}
		deleteNode(current);

		_currentCapacity--;
		return true;
	}
}

template <typename KeyType, typename ValueType>
unsigned int HashTable<KeyType, ValueType>::getBucketForKey(const KeyType& key) const
{
	unsigned int computeHash(KeyType);
	return computeHash(key) % _numBuckets;
}

template <typename KeyType, typename ValueType>
bool HashTable<KeyType, ValueType>::touchNode(Node* node)
{
	if (node != nullptr && !node->permanent)
	{
		if (node != _hist_last_modified)
		{
			if (node == _hist_first_modified)
			{
				//Set the next node as the first node to be modified
				_hist_first_modified = node->hist_later;
				_hist_first_modified->hist_before = node;
			}
			else
			{
				//Link the nodes surrounding the touched node to each other
				Node* hist_later = node->hist_later;
				Node* hist_before = node->hist_before;
				hist_before->hist_later = hist_later;
				hist_later->hist_before = hist_before;
			}
			//Move the node to the back of the linked list
			_hist_last_modified->hist_later = node;
			node->hist_before = _hist_last_modified;
			node->hist_later = _hist_first_modified;
			_hist_last_modified = node;
			
		}
		return true;
	}
	return false;
}

template <typename KeyType, typename ValueType>
void HashTable<KeyType, ValueType>::deleteNode(Node* node)
{
	if (node->next == node) //If this condition is true, the dummy node of a linked list was passed in due to a bug. Don't delete it.
		return;

	Node* current = node->next;
	Node* prev = node->prev;
	delete node;
	prev->next = current;
	current->prev = prev;
}

template <typename KeyType, typename ValueType>
HashTable<KeyType, ValueType>::LinkedList::LinkedList()
{
	_head = new Node;
	_head->prev = _head;
	_head->next = _head;
}

template <typename KeyType, typename ValueType> 
HashTable<KeyType, ValueType>::LinkedList::~LinkedList() //Deletes all nodes in the linked list
{
	Node* current = _head;
	while (true)
	{
		Node* next = current->next;
		delete current;
		if (next == _head)
			break;
		current = next;
	}
}

template <typename KeyType, typename ValueType>
typename HashTable<KeyType, ValueType>::Node* HashTable<KeyType, ValueType>::LinkedList::insert(const KeyType& key, const ValueType& value, bool permanent)
{
	Node* current = _head->next;
	while (true)
	{
		if (current->next == _head) //Reached the end of the bucket without finding the key, so add a new node
		{
			Node* prev = current;
			current = new Node;
			current->prev = prev;
			current->next = _head;
			current->key = key;
			current->permanent = permanent;
			prev->next = current;
			_head->prev = current;
			break;
		}
		else if (current->key == key)
			break;
		current = current->next;
	}

	current->value = value;
	return current;
}

//template <typename KeyType, typename ValueType>
//void HashTable<KeyType, ValueType>::LinkedList::erase(const KeyType& key)
//{
//	Node* current = _head->next;
//	while (true)
//	{
//		if (current == _head)
//			break;
//		else if (current->key == key)
//		{
//			deleteNode(current);
//			break;
//		}
//		current = current->next;
//	}
//}
//
//template<typename KeyType, typename ValueType>
//bool HashTable<KeyType, ValueType>::LinkedList::empty() const
//{
//	return _head->next == _head;
//}

template<typename KeyType, typename ValueType>
typename HashTable<KeyType, ValueType>::Node* HashTable<KeyType, ValueType>::LinkedList::get(const KeyType& key) const
{
	Node* current = _head->next;
	while (true)
	{
		if (current == _head)
			return nullptr;
		else if (current->key == key)
			return current;
		current = current->next;
	}
}

#endif