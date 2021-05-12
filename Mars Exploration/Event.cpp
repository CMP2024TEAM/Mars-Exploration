#include "Event.h"

Event::Event(int ED, int ID)
{
	EventDay = ED;
	MissionId = ID;
}

int Event::GetEventDay()
{
	return EventDay;
}

Event::~Event()
{
}