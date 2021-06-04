#pragma once
#include "enums.h"
class Rover
{
public:
	static int LastId;
	static int CheckupDuration[3];
	Rover(int);
	virtual ~Rover();
	int GetID();
	virtual RoverType getType() const = 0;
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
	void ResetCompletedMissions();
private:
	int Id;
	int Speed;
	int CompletedMissions;
	int AvailableAt;
	RoverStatus Status;
};