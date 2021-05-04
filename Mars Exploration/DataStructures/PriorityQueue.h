#pragma once
#include"QueueInterface.h"
#include"pair.h"
#include"node.h"

/* Stores elements in a queue sorted descendingly(Max is front) according to priority.
Priority is determined through an integer that is passed with each queue
object in a pair struct. */
template<class T>
class PriorityQueue : public QueueInterface<MyPair<T, int>> {
private:
    /*Head pointer to sorted linked chain
    the chain is sorted descedingly according to pair.second*/
    node<MyPair<T, int>>* FrontPtr;

    //Delete all nodes in linked chain
    void clearAll();

    //Clone from an exisiting linked list
    void cloneFromOther(node<MyPair<T, int>>* otherHead);
public:
    PriorityQueue();

    PriorityQueue(const PriorityQueue<T>& other);
    PriorityQueue<T>& operator=(const PriorityQueue<T>& other);

    /* Check if queue is empty
    O(1)*/
    bool isEmpty() const;

    /* Insert element into queue. 
    O(n)*/
    bool enqueue(const MyPair<T, int>& newEntry);

    /* Remove element from front of queue if possible. 
    the removed element is stored in frontEntry
    O(1)*/
    bool dequeue(MyPair<T, int>& frontEntry);

    /* Store the front entry in frontEntry if available
    O(1)*/
    bool peekFront(MyPair<T, int>& frontEntry) const;

    ~PriorityQueue();
};


//Delete all nodes in linked chain
template<class T>
inline void PriorityQueue<T>::clearAll()
{
    node<MyPair<T, int>>* Temp;
    while (FrontPtr != nullptr) {
        Temp = FrontPtr;
        FrontPtr = FrontPtr->next;
        delete Temp;
    }
}

template<class T>
inline void PriorityQueue<T>::cloneFromOther(node<MyPair<T, int>>* otherHead)
{
    if (otherHead == nullptr) {
        FrontPtr = nullptr;
        return;
    }

    FrontPtr = new node<MyPair<T, int>>(otherHead->val);
    node<MyPair<T, int>>* ToBeCopied = otherHead->next;
    node<MyPair<T, int>>* Curr = FrontPtr;
    while (ToBeCopied != nullptr) {
        Curr->next = new node<MyPair<T, int>>(ToBeCopied->val);
        Curr = Curr->next;
        ToBeCopied = ToBeCopied->next;
    }
}

template<class T>
inline PriorityQueue<T>::PriorityQueue() :
    FrontPtr(nullptr)
{}

template<class T>
inline PriorityQueue<T>::PriorityQueue(const PriorityQueue<T>& other)
{
    cloneFromOther(other.FrontPtr);
}

template<class T>
inline PriorityQueue<T>& PriorityQueue<T>::operator=(const PriorityQueue<T>& other)
{
    clearAll();
    cloneFromOther(other.FrontPtr);
    return *this;
}

template<class T>
inline bool PriorityQueue<T>::isEmpty() const
{
    return FrontPtr == nullptr;
}

template<class T>
inline bool PriorityQueue<T>::enqueue(const MyPair<T, int>& newEntry)
{
    //Queue was empty. Element is inserted at the begining
    if (FrontPtr == nullptr) {
        FrontPtr = new node<MyPair<T, int>>(newEntry);
        return FrontPtr == nullptr;
    }

    //Element is inserted at the begining
    if (FrontPtr->val.second < newEntry.second) {
        if (node<MyPair<T, int>>* Temp = new node<MyPair<T, int>>(newEntry, FrontPtr)) {
            FrontPtr = Temp;
            return true;
        }

        return false;
    }

    //Element is inserted in the middle
    node<MyPair<T, int>>* Curr = FrontPtr;
    while (Curr->next != nullptr) {
        if (Curr->next->val.second < newEntry.second) {
            if (node<MyPair<T, int>>* Temp = new node<MyPair<T, int>>(newEntry, Curr->next)) {
                Curr->next = Temp;
                return true;
            }

            return false;
        }
        Curr = Curr->next;
    }

    //Element is inserted at the end
    if (node<MyPair<T, int>>* Temp = new node<MyPair<T, int>>(newEntry)) {
        Curr->next = Temp;
        return true;
    }

    return false;
}

template<class T>
inline bool PriorityQueue<T>::dequeue(MyPair<T, int>& frontEntry)
{
    if (FrontPtr == nullptr)
        return false;

    //Assign current front entry to passed object
    frontEntry = FrontPtr->val;

    //Delete front entry
    node<MyPair<T, int>>* Temp = FrontPtr;
    FrontPtr = FrontPtr->next;
    delete Temp;

    return true;
}

template<class T>
inline bool PriorityQueue<T>::peekFront(MyPair<T, int>& frontEntry) const
{
    if (FrontPtr == nullptr)
        return false;

    //Assign current front entry to passed object
    frontEntry = FrontPtr->val;

    return true;
}

template<class T>
inline PriorityQueue<T>::~PriorityQueue()
{
    clearAll();
}
