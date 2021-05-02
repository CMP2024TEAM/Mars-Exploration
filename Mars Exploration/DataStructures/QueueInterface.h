#pragma once

template<class ItemType>
class QueueInterface {
public:
    /* Sees whether the queue is empty*/
    virtual bool isEmpty() const = 0;

    /* Adds a new entry to the back of this queue*/
    virtual bool enqueue(const ItemType& newEntry) = 0;

    /* Removes the front of this queue.
    stores removed item in newEntry */
    virtual bool dequeue(ItemType& frontEntry) = 0;
    
    /* Returns the front of this queue*/
    virtual ItemType peekFront() const = 0;
}; // end QueueInterface