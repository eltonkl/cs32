// Multiset.cpp

#include "Multiset.h"

void exchange(int& a, int& b)
{
    int t = a;
    a = b;
    b = t;
}

template <typename T>
Multiset<T>::Multiset()
 : m_uniqueSize(0), m_size(0)
{
      // create dummy node
    m_head = new Node;
    m_head->m_next = m_head;
    m_head->m_prev = m_head;
}

template <typename T>
Multiset<T>::~Multiset()
{
    // Delete the m_uniqueSize non-dummy nodes plus the dummy node
    for (Node* p = m_head->m_prev ; m_uniqueSize >= 0; m_uniqueSize--)
    {
		Node* toBeDeleted = p;
		p = p->m_prev;
		delete toBeDeleted;
    }
}

template <typename T>
Multiset<T>::Multiset(const Multiset& other)
 : m_uniqueSize(other.m_uniqueSize), m_size(other.m_size)
{
      // Create dummy node; don't initialize its m_next

    m_head = new Node;
    m_head->m_prev = m_head;

      // Copy each node from the other list; each iteration will set the
      // m_next of the previous node copied

	for (Node* p = other.m_head->m_next; p != other.m_head; p = p->m_next)
	{
		// Create a copy of the node p points to
		Node* pnew = new Node;
		pnew->m_value = p->m_value;
		pnew->m_count = p->m_count;

		// Connect the m_prev pointers
		pnew->m_prev = m_head->m_prev;
		m_head->m_prev = pnew;

		// Connect the previous Node's m_next 
		pnew->m_prev->m_next = pnew;
	}

      // Connect the last Node's m_next 
    m_head->m_prev->m_next = m_head;
}

template <typename T>
Multiset<T>& Multiset<T>::operator=(const Multiset<T>& rhs)
{
    if (this != &rhs)
    {
	Multiset temp(rhs);
	swap(temp);
    }
    return *this;
}

template <typename T>
bool Multiset<T>::insert(const T& value)
{
    Node* p = find(value);

    if (p != m_head)  // found
        p->m_count++;
    else
    {
	  // Create a new node
	p = new Node;
	p->m_value = value;
	p->m_count = 1;

	  // Insert new item at tail of list (arbitrary choice of position)
	  //     Connect it to tail
	p->m_prev = m_head->m_prev;
	p->m_prev->m_next = p;

	  //     Connect it to dummy node
	p->m_next = m_head;
	m_head->m_prev = p;

	m_uniqueSize++;
    }

    m_size++;
    return true;
}

template <typename T>
int Multiset<T>::count(const T& value) const
{
    Node* p = find(value);
    return p == m_head ? 0 : p->m_count;
}
     
template <typename T>
int Multiset<T>::get(int i, T& value) const
{
    if (i < 0  ||  i >= m_uniqueSize)
        return false;

      // Get the value at position i.  This is one way of ensuring the required
      // behavior of get:  If the Multiset doesn't change in the interim,
      // * calling get with each i in 0 <= i < size() gets each of the
      //   Multiset elements, and
      // * calling get with the same value of i each time gets the same element.

      // If i is closer to the head of the list, go forward to reach that
      // position; otherwise, start from tail and go backward.

    Node* p;
    if (i < m_uniqueSize / 2)  // closer to head
    {
        p = m_head->m_next;
        for (int k = 0; k != i; k++)
            p = p->m_next;
    }
    else  // closer to tail
    {
        p = m_head->m_prev;
        for (int k = m_uniqueSize-1; k != i; k--)
            p = p->m_prev;
    }

    value = p->m_value;
    return p->m_count;
}

template <typename T>
void Multiset<T>::swap(Multiset& other)
{
      // swap head pointers
    Node* temp = m_head;
    m_head = other.m_head;
    other.m_head = temp;

      // swap uniqueSize and size
    exchange(m_uniqueSize, other.m_uniqueSize);
    exchange(m_size, other.m_size);
}

template <typename T>
Multiset<T>::Node* Multiset<T>::find(const T& value) const
{
      // Do a linear search through the list

    Node* p;
    for (p = m_head->m_next; p != m_head && p->m_value != value; p = p->m_next)
	;
    return p;
}

template <typename T>
int Multiset<T>::doErase(const T& value, bool all)
{
    Node* p = find(value);

    if (p == m_head)  // not found
	return 0;

    int nErased = (all ? p->m_count : 1);  // number to erase

      // If erasing one, and there are more than one, just decrement;
      // otherwise, we're erasing all, or erasing one whose count is 1,
      // so unlink the Node from the list and destroy it

    if (!all  &&  p->m_count > 1)
	p->m_count--;
    else
    {
        p->m_prev->m_next = p->m_next;
        p->m_next->m_prev = p->m_prev;
        delete p;

        m_uniqueSize--;
    }

    m_size -= nErased;
    return nErased;
}

template <typename T>
void combine(const Multiset<T>& ms1, const Multiset<T>& ms2, Multiset<T>& result)
{
      // Guard against the case that result is an alias for ms1 or ms2
      // (i.e., that result is a reference to the same multiset that ms1 or ms2
      // refers to) by building the answer in a local variable res.  When
      // done, swap res with result; the old value of result (now in res) will
      // be destroyed when res is destroyed.

    Multiset res(ms1);
    for (int k = 0; k < ms2.uniqueSize(); k++)
    {
        ItemType v;
        for (int n = ms2.get(k, v); n > 0; n--)
	    res.insert(v);
    }
    result.swap(res);
}

template <typename T>
void subtract(const Multiset<T>& ms1, const Multiset<T>& ms2, Multiset<T>& result)
{
      // Guard against the case that result is an alias for ms1 or ms2
      // by building the answer in a local variable res.  When done, swap res
      // with result; the old value of result (now in res) will be destroyed
      // when res is destroyed.

    Multiset res;
    for (int k = 0; k != ms1.uniqueSize(); k++)
    {
	ItemType v;
        int n = ms1.get(k, v);
	for (n -= ms2.count(v); n > 0; n--)
	    res.insert(v);
    }
    result.swap(res);
}
