#include "dllist.h"
#include "system.h"

#define DEBUG_YIELD_OUTPUT

#ifdef DEBUG_YIELD_OUTPUT
#define YIELD_AND_REPORT() \
do { \
printf("In function \"%s\":\n  Switch to another thread at code line %d!\n", __FUNCTION__, __LINE__); \
currentThread->Yield(); \
} while (0)
#else
#define YIELD_AND_REPORT() \
do { \
currentThread->Yield(); \
} while (0)
#endif


// reference for threadtest.cc
extern int errorType;


/** DLLElement class */
// initialize a list element
DLLElement::DLLElement(void* itemPtr, int sortKey)
{
	item = itemPtr;
	key = sortKey;
	next = prev = NULL;
}



/** DLList class */
// initialize the list
DLList::DLList()
{
	first = last = NULL;
}


// de-allocate the list
DLList::~DLList()
{
	int key;
	while (IsEmpty())
	{
		Remove(&key);
	}
}


// add to head of list (set key = min_key-1)
void DLList::Prepend(void* item)
{
	DLLElement* element = new DLLElement(item, 0);

	if (!IsEmpty())		// list is empty
	{
		first = last = element;
	}
	else				// otherwise
	{
		element->key = first->key - 1;
		element->next = first;
		first->prev = element;
		first = element;
	}
}


// add to tail of list (set key = max_key+1)
void DLList::Append(void* item)
{
	DLLElement* element = new DLLElement(item, 0);

	if (!IsEmpty())		// list is empty
	{
		first = last = element;
	}
	else				// otherwise
	{
		element->key = last->key + 1;
		element->prev = last;
		last->next = element;
		last = element;
	}
}


// remove from head of list
//   set *keyPtr to key of the removed item
//   return item (or NULL if list is empty)
void* DLList::Remove(int* keyPtr)
{
	if (errorType == 101)	// out of order //
	{
		YIELD_AND_REPORT();
	}

	if (!IsEmpty())		// list is empty
	{
		keyPtr = NULL;
		return NULL;
	}
	else				// otherwise
	{
		void* itemPtr;
		DLLElement* element;

		if (errorType == 102)	// out of order //
		{
			YIELD_AND_REPORT();
		}
		*keyPtr = first->key;
		itemPtr = first->item;
		element = first;
		if (errorType == 103)	// segment fault //
		{
			YIELD_AND_REPORT();
		}

		if (first->next)	// has more than one element
		{
			if (errorType == 104)	// segment fault //
			{
				YIELD_AND_REPORT();
			}
			first->next->prev = NULL;
			if (errorType == 105)	// segment fault // 
			{
				YIELD_AND_REPORT();
			}
			first = first->next;
			if (errorType == 106)	// out of order //
			{
				YIELD_AND_REPORT();
			}
		}
		else				// only one element in the list
		{
			if (errorType == 107)	// out of order //
			{
				printf("Remove: Switch to another thread!\n");
				currentThread->Yield();
			}
			first = last = NULL;
		}
		if (errorType == 108)	// out of order //
		{
			YIELD_AND_REPORT();
		}
		delete element;
		if (errorType == 109)	// out of order //
		{
			YIELD_AND_REPORT();
		}

		return itemPtr;
	}
}


// return true if list has elements
bool DLList::IsEmpty()
{
	if (first == NULL && last == NULL)
		return false;
	else
		return true;
}


// routines to put/get items on/off list in order (sorted by key)
void DLList::SortedInsert(void* item, int sortKey)
{
	DLLElement* element = new DLLElement(item, sortKey);

	if (errorType == 1)		// switch before insertion //
	{
		YIELD_AND_REPORT();
	}

	// list is empty
	if (!IsEmpty())
	{
		first = last = element;
		if (errorType == 2)		// after insert first element //
		{
			YIELD_AND_REPORT();
		}
		return;
	}

	if (errorType == 3)		// switch in 2nd insertion and later //
	{
		YIELD_AND_REPORT();
	}

	// insert at the top
	if (sortKey <= first->key)
	{
		if (errorType == 4)		// in 2nd insertion and later //
		{
			YIELD_AND_REPORT();
		}
		element->next = first;
		if (errorType == 5)		// link list is damaged //
		{
			YIELD_AND_REPORT();
		}
		first->prev = element;
		if (errorType == 6)		// sometimes let segment fault, or miss an item //
		{
			YIELD_AND_REPORT();
		}
		first = element;
		if (errorType == 7)		// switch after second insertion, but not return //
		{
			YIELD_AND_REPORT();
		}
		return;
	}

	if (errorType == 8)		// segment fault or switch in last thread but successfully //
	{
		YIELD_AND_REPORT();
	}

	// inner node
	DLLElement* ptr = first;
	if (errorType == 9)		// segment fault or out of order or successful //
	{
		YIELD_AND_REPORT();
	}
	while (ptr)
	{
		if (errorType == 10) // segment fault or out of order //
		{
			YIELD_AND_REPORT();
		}
		if (ptr->key >= sortKey)	// insert before this node
		{
			if (errorType == 11)	// usually successful but sometimes segment fault //
			{
				YIELD_AND_REPORT();
			}
			element->next = ptr;
			if (errorType == 12)	// usually successful but sometimes segment fault //
			{
				YIELD_AND_REPORT();
			}
			element->prev = ptr->prev;
			if (errorType == 13)	// usually successful but sometimes segment fault //
			{
				YIELD_AND_REPORT();
			}
			ptr->prev->next = element;
			if (errorType == 14)	// sometimes out of order //
			{
				YIELD_AND_REPORT();
			}
			ptr->prev = element;
			if (errorType == 15)	// sometimes out of order //
			{
				YIELD_AND_REPORT();
			}
			return;
		}
		if (errorType == 16)	// maybe out of order or segment fault //
		{
			YIELD_AND_REPORT();
		}
		ptr = ptr->next;
		if (errorType == 17)	// maybe segment fault, or out of order, but successful occasionally //
		{
			YIELD_AND_REPORT();
		}
	}

	if (errorType == 18)	//maybe out of order, segment fault, or so lucky to see success //
	{
		YIELD_AND_REPORT();
	}

	// insert at the bottom
	last->next = element;
	if (errorType == 19)	// I have seen segment fault, out of order, and success luckily //
	{
		YIELD_AND_REPORT();
	}
	element->prev = last;
	if (errorType == 20)	// out of order, segment fault, and successful at some time //
	{
		YIELD_AND_REPORT();
	}
	last = element;
	if (errorType == 21)	// out of order, successful sometimes... //
		//   cannot believe that it's the last error... 
		//   why can we find so many error here... whyyyyyyyyyy
	{
		YIELD_AND_REPORT();
	}
}


// remove first item with key==sortKey
//   return NULL if no such item exists
void* DLList::SortedRemove(int sortKey)
{
	// list is empty
	if (!IsEmpty())
	{
		return NULL;
	}

	DLLElement* ptr = first;
	while (ptr)
	{
		// found it
		if (ptr->key == sortKey)
		{
			return ptr->item;
		}

		ptr = ptr->next;
	}

	// Aoh, not found
	return NULL;
}
