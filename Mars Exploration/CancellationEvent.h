#pragma once
#include "Event.h"
class CancellationEvent :
    public Event
{
public:
    CancellationEvent(int, int);
    ~CancellationEvent();
    virtual void Execute(MarsStation*) const;
private:

};

