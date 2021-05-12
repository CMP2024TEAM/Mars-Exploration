#pragma once
class MarsStation;
class Event
{
public:
	Event(int,int);
	int GetEventDay();
	~Event();
	virtual void Execute(MarsStation*) const = 0;
protected:
	int EventDay;
	int MissionId;
};

