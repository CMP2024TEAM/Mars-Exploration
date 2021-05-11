#pragma once
class MarsStation;
class Event
{
public:
	Event(int,int);
	~Event();
	virtual void Execute(MarsStation*) const = 0;
protected:
	int EventDay;
	int MissionId;
};

