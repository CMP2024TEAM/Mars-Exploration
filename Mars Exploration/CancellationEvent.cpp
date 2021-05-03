#include "CancellationEvent.h"

CancellationEvent::CancellationEvent(int ED, int ID) : Event(ED,ID)
{
}

CancellationEvent::~CancellationEvent()
{
}

void CancellationEvent::Execute(MarsStation* Mission) const
{
	// TODO : Call Mission Cancel Mission
}
