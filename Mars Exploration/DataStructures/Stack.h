#pragma once
#include"node.h"
#include "StackInterface.h"
template< class T>
class Stack : public StackInterface<T>
{
private:
	node<T>* topPtr; // Pointer to first node in the chain;
	// this node contains the stack’s top
	void clearAll(); //utility function

public:
	// Constructors and destructor:
	Stack(); // Default constructor
	Stack(const Stack<T>& aStack); // Copy constructor
	virtual ~Stack(); // Destructor
	// Stack operations:
	bool isEmpty() const;
	bool push(const T& newItem);
	bool pop(T&);
	bool peek(T&) const;
}; // end Stack

template< class T>
Stack<T>::Stack() : topPtr(nullptr)
{
} // end default constructor
template< class T>
Stack<T>::
Stack(const Stack<T>& aStack)
{
	// Point to nodes in original chain
	node<T>* origChainPtr = aStack.topPtr;
	if (origChainPtr == nullptr)
		topPtr = nullptr; // Original bag is empty
	else
	{
		// Copy first node
		topPtr = new node<T>();
		topPtr->val = (origChainPtr->val);
		// Point to first node in new chain
		node<T>* newChainPtr = topPtr;
		// Copy remaining nodes
		while (origChainPtr != nullptr&& origChainPtr->next !=nullptr)
		{
			// Advance original-chain pointer
			origChainPtr = origChainPtr->next;
			// Get next item from original chain
			T nextItem = origChainPtr->val;
			// Create a new node containing the next item
			node<T>* newnodePtr = new node<T>(nextItem);
			// Link new node to end of new chain
			newChainPtr->next = (newnodePtr);
			// Advance pointer to new last node
			newChainPtr = newChainPtr->next;
		} // end while
		newChainPtr->next = (nullptr); // Flag end of chain
	} // end if
} // end copy constructor
template< class T>
Stack<T>::~Stack()
{
	// Pop until stack is empty
	clearAll();
} // end destructor
template< class T>
bool Stack<T>::isEmpty() const
{
	return topPtr == nullptr;
} // end isEmpty
template< class T>
bool Stack<T>::push(const T& newItem)
{
	node<T>* newnodePtr = new node<T>(newItem, topPtr);
	topPtr = newnodePtr;
	newnodePtr = nullptr; return true;
} // end push
template< class T>
bool Stack<T>::pop(T &Item)
{
	bool result = false;
	if (!isEmpty())
	{
		Item = topPtr->val;
		// Stack is not empty; delete top
		node<T>* nodeToDeletePtr = topPtr;
		topPtr = topPtr->next;
		// Return deleted node to system
		nodeToDeletePtr->next = nullptr;
		delete nodeToDeletePtr;
		nodeToDeletePtr = nullptr;
		result = true;
	} // end if
	return result;
} // end pop
template< class T>
bool Stack<T>::peek(T &Item) const
{
	if (!isEmpty())
	{// Stack is not empty; return top
		Item=topPtr->val;
		return true;	
	}
	else
	{
		return false;
	}
} // end getTop
// end of implementation file
template<typename T>
inline void Stack<T>::clearAll()
{
	node<T>* Temp = topPtr;
	while (topPtr) {
		topPtr = topPtr->next;
		delete Temp;
		Temp = topPtr;
	}
}
