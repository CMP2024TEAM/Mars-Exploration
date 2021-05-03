#pragma once
#include "enums.h"
class Rover
{
public:
	static int LastId;
	static int CheckupDuration[3];
	Rover(RoverType,int);
	~Rover();
	void setType(RoverType);
	RoverType getType() const;
	void setCheckUpDuration(int);
	int getCheckUpDuration();
	void setSpeed(int);
	int getSpeed() const;
	void increaseCompletedMissions(int = 1);
	int getCompletedMissions() const;
	void setAvailableAt(int);
	int getAvailableAt() const;
	void setStatus(RoverStatus);
	RoverStatus getStatus() const;
private:
	int Id;
	RoverType Type;
	int Speed;
	int CompletedMissions;
	int AvailableAt;
	RoverStatus Status;
};