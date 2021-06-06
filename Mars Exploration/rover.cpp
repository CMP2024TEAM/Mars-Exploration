#include "Rover.h"
#include<iostream>
int Rover::LastId = 0;
int Rover::CheckupDuration[3] = {0};

Rover::Rover(int speed)
{
	Id = ++LastId;
	Status = RoverStatus::Available;
	CompletedMissions = 0;
	Speed = speed;
	AvailableAt = 0;
	Requsted = 0;
}
Rover::~Rover()
{

}

int Rover::GetID()
{
	return Id;
}

void Rover::setCheckUpDuration(int c)
{
	CheckupDuration[(int)getType()] = c;
}

int Rover::getCheckUpDuration()
{
	return CheckupDuration[(int)getType()];
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

void Rover::ResetCompletedMissions()
{
	CompletedMissions = 0;
}

int Rover::GetHealth() const
{
	return Health;
}

void Rover::DecrementSpeed()
{
	Speed /= 2;
	if (Speed < 1) Speed = 1;
	std::cout << "\nRover speed was decremented to " << Speed << " ID= " << Id <<std::endl;;
}

void Rover::IncrementRequsted()
{
	Requsted++;
}

int Rover::GetRequsted()
{
	return Requsted;
}

void Rover::ResetRequsted()
{
	Requsted = 0;
}
