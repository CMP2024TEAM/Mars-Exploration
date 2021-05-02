#pragma once

template<class ItemType>
class QueueInterface {
public:
    /* Sees whether the queue is empty*/
    virtual bool isEmpty() const = 0;

    /* Adds a new entry to the back of this queue*/
    virtual bool enqueue(const ItemType& newEntry) = 0;

    /* Removes the front entry of this queue.
    stores removed item in frontEntry*/
    virtual bool dequeue(ItemType& frontEntry) = 0;
    
    /* stores front item in frontEntry*/
    virtual bool peekFront(ItemType& frontEntry) const = 0;

    virtual ~QueueInterface() = 0 {};
}; // end QueueInterface