#include "Rover.h"
int Rover::LastId = 0;
int Rover::CheckupDuration[3] = {0};

Rover::Rover(RoverType type,int speed)
{
	Id = ++LastId;
	Type = type;
	Status = RoverStatus::Available;
	CompletedMissions = 0;
	Speed = speed;
	AvailableAt = 0;
}
Rover::~Rover()
{

}

int Rover::GetID()
{
	return Id;
}

void Rover::setType(RoverType type)
{
	Type = type;
}

RoverType Rover::getType() const
{
	return Type;
}

void Rover::setCheckUpDuration(int c)
{
	CheckupDuration[(int)Type] = c;
}

int Rover::getCheckUpDuration()
{
	return CheckupDuration[(int)Type];
}

void Rover::setSpeed(int s)
{
	Speed = s;
}

int Rover::getSpeed() const
{
	return Speed;
}

void Rover::increaseCompletedMissions(int by)
{
	CompletedMissions += by;
}

int Rover::getCompletedMissions() const
{
	return CompletedMissions;
}

void Rover::setAvailableAt(int at)
{
	AvailableAt = at;
}

int Rover::getAvailableAt() const
{
	return AvailableAt;
}

void Rover::setStatus(RoverStatus status)
{
	Status = status;
}

RoverStatus Rover::getStatus() const
{
	return Status;
}
