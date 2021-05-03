#pragma once
#include "enums.h"
class rover
{
public:
	static int LastId;
	static int CheckupDuration[3];
	rover(rovertype,int);
	~rover();
	void setType(rovertype);
	rovertype getType() const;
	void setCheckUpDuration(int);
	int getCheckUpDuration() const;
	void setSpeed(int);
	int getSpeed() const;
	void increaseCompletedMissions(int = 1);
	int getCompletedMissions() const;
	void setAvailableAt(int);
	int getAvailableAt() const;
	void setStatus(roverstatus);
	roverstatus getStatus() const;
private:
	int Id;
	rovertype Type;
	int Speed;
	int CompletedMissions;
	int AvailableAt;
	roverstatus Status;
};