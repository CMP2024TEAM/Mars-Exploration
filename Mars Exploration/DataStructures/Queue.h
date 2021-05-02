#pragma once
#include"QueueInterface.h"
#include"node.h"
template<typename T>
class Queue : public QueueInterface<T>
{
private:
	// The queue is implemented as a chain of linked nodes that has
	// two external pointers, a head pointer for the front of the queue
	// and a tail pointer for the back of the queue.
	node<T>* backPtr;
	node<T>* frontPtr;
	clearAll()
	{
		node<T> Temp=frontPtr;
		while (frontPtr)
		{
			frontPtr = frontPtr->next;
			delete Temp;
			Temp = frontPtr;
		}
	}
public:
	Queue():backPtr(nullptr),frontPtr(nullptr)
	{
	}
	Queue(const Queue& aQueue)
	{
		node<T> Ptr = aQueue.frontPtr;
		while (Ptr)
		{
			enqueue(Ptr->val);
			Ptr = Ptr->next;
		}
	}
	~Queue()
	{
		clearAll();
	}
	bool isEmpty() const
	{
		return (backPtr!=frontPtr);
	}
	bool enqueue(const T & newEntry) 
	{
		node<T>* newNodePtr = new node<T>(newEntry);
		// Insert the new node
		if (isEmpty())
			frontPtr = newNodePtr; // The queue was empty
		else
			backPtr->next=newNodePtr; // The queue was not empty
		backPtr = newNodePtr; // New node is at back
		return true;
	} // end enqueue
	bool dequeue(T & newEntry) 
	{
		bool result = false;
		if (!isEmpty())
		{
			newEntry = frontPtr->val;
			// Queue is not empty; remove front
			node<T>* nodeToDeletePtr = frontPtr;
			if (frontPtr == backPtr)
			{ // Special case: one node in queue
				frontPtr = nullptr;
				backPtr = nullptr;
			}
			else
				frontPtr = frontPtr->next;
			// Return deleted node to system
			nodeToDeletePtr->next=nullptr;
			delete nodeToDeletePtr;
			nodeToDeletePtr = nullptr;
			result = true;
		} // end if
		return result;
	} // end dequeue
	bool peekFront(T & value)
	{
		if (isEmpty())
			return false;
		value = frontPtr->val;
		return true;
	}
};

