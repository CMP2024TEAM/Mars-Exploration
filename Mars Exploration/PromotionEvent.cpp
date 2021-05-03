#include "PromotionEvent.h"

PromotionEvent::PromotionEvent(int ED, int ID):Event(ED,ID)
{
}

PromotionEvent::~PromotionEvent()
{
}

void PromotionEvent::Execute(MarsStation*) const
{
	// TODO : Call Mission Promote Mission
}
