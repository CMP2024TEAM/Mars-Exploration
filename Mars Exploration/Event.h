#pragma once
#include "MarsStation.h"
class Event
{
public:
	Event(int,int);
	~Event();
	virtual void Execute(MarsStation*) const = 0;
private:
	int EventDay;
	int MissionId;
};

