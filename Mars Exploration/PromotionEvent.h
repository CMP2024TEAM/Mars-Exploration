#pragma once
#include "Event.h"
class PromotionEvent :
    public Event
{
public:
    PromotionEvent(int, int);
    ~PromotionEvent();
    virtual void Execute(MarsStation*) const;
private:
};

