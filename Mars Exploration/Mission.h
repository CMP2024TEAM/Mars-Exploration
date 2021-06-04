#pragma once
#include "enums.h"
#include "Rover.h"
class Mission
{
private:
	int Id;
	int FormulationDay;	
	int TargetLocation;		//The distance (in kilometers)
	static int ID;
	int MissionDuration;
	int WaitingDays;
	MissionStatus Status;
	Rover* AssignedRover;
	int ED;
	int Get_ED()		const;			//execution days=2*(Target location/Rover Speed)+Mission Duration

public:

	Mission(int FD,int TL,int MD);
	int GetFormulationDay()		const;
	void SetFormulationDay(int FD);
	int GetTargetLocation ()		const;
	void SetTargetLocation (int TL);
	int GetMissionDuration()		const;
	void SetMissionDuration(int MD);
	int GetID()		const;
	int GetWaitingDays()		const;
	void SetWaitingDays(int WD);
	int GetCD()		const;			//Compeleting days=FD+WD+ED
	void AssignRover(Rover* R);
	Rover* GetRover()		const;
	void DismissRover();
	void SetMissionStatus(MissionStatus status);
	MissionStatus GetMissionStatus ()		const;
	virtual MissionType GetMissionType() const=0;
	void SetED();
	int GetED() const;
	~Mission(void);
};

