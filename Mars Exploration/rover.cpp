#pragma once
#include "rover.h"
int rover::LastId = 0;
rover::rover(rovertype type,int checkup, int speed)
{
	Id = ++LastId;
	Type = type;
	Status = roverstatus::Available;
	CompletedMissions = 0;
	CheckUpDuration = checkup;
	Speed = speed;
	AvailableAt = 0;
}
rover::~rover()
{

}

void rover::setType(rovertype type)
{
	Type = type;
}

rovertype rover::getType() const
{
	return Type;
}

void rover::setCheckUpDuration(int c)
{
	CheckUpDuration = c;
}

int rover::getCheckUpDuration() const
{
	return CheckUpDuration;
}

void rover::setSpeed(int s)
{
	Speed = s;
}

int rover::getSpeed() const
{
	return Speed;
}

void rover::increaseCompletedMissions(int by)
{
	CompletedMissions += by;
}

int rover::getCompletedMissions() const
{
	return CompletedMissions;
}

void rover::setAvailableAt(int at)
{
	AvailableAt = at;
}

int rover::getAvailableAt() const
{
	return AvailableAt;
}

void rover::setStatus(roverstatus status)
{
	Status = status;
}

roverstatus rover::getStatus() const
{
	return Status;
}
