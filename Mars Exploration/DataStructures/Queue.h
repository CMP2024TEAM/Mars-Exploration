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
	void clearAll();
public:
	Queue();
	Queue(const Queue& aQueue);

	~Queue();

	bool isEmpty() const;
	bool enqueue(const T& newEntry); // end enqueue
	bool dequeue(T& newEntry); // end dequeue
	bool peekFront(T& value) const;
};

template<typename T>
inline void Queue<T>::clearAll()
{
	node<T>* Temp = frontPtr;
	while (frontPtr) {
		frontPtr = frontPtr->next;
		delete Temp;
		Temp = frontPtr;
	}
}

template<typename T>
inline Queue<T>::Queue() :
	backPtr(nullptr), 
	frontPtr(nullptr)
{}

template<typename T>
inline Queue<T>::Queue(const Queue & aQueue)
{
	node<T>* Ptr = aQueue.frontPtr;
	while (Ptr) {
		enqueue(Ptr->val);
		Ptr = Ptr->next;
	}
}

template<typename T>
inline Queue<T>::~Queue()
{
	clearAll();
}

template<typename T>
inline bool Queue<T>::isEmpty() const
{
	return frontPtr == nullptr;
}

template<typename T>
inline bool Queue<T>::enqueue(const T& newEntry)
{
	node<T>* newNodePtr = new node<T>(newEntry);
	// Insert the new node
	if (isEmpty())
		frontPtr = newNodePtr; // The queue was empty
	else
		backPtr->next = newNodePtr; // The queue was not empty
	backPtr = newNodePtr; // New node is at back
	return true;
} // end enqueue

template<typename T>
inline bool Queue<T>::dequeue(T& newEntry)
{
	bool result = false;
	if (!isEmpty()) {
		newEntry = frontPtr->val;
		// Queue is not empty; remove front
		node<T>* nodeToDeletePtr = frontPtr;
		if (frontPtr == backPtr) { // Special case: one node in queue
			frontPtr = nullptr;
			backPtr = nullptr;
		}
		else
			frontPtr = frontPtr->next;
		// Return deleted node to system
		nodeToDeletePtr->next = nullptr;
		delete nodeToDeletePtr;
		nodeToDeletePtr = nullptr;
		result = true;
	} // end if
	return result;
} // end dequeue

template<typename T>
inline bool Queue<T>::peekFront(T& value) const
{
	if (isEmpty())
		return false;
	value = frontPtr->val;
	return true;
}
