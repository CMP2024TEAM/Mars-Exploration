#pragma once
#include "enums.h"
#include "Rover.h"
class Mission
{
private:
	int FormulationDay;	
	int TargetLocation;		//The distance (in kilometers)
	int Significance;		//representing the importance of the mission
	static int ID;
	int MissionDuration;
	int WaitingDays;
	MissionType Type;
	MissionStatus Status;
	Rover* AssignedRover;
	
public:

	Mission(int FD,int TL,int S,int MD,MissionType T);
	int GetFormulationDay()		const;
	void SetFormulationDay(int FD);
	int GetTargetLocation ()		const;
	void SetTargetLocation (int TL);
	int GetSignificance()		const;
	void SetSignificance(int S);
	int GetMissionDuration()		const;
	void SetMissionDuration(int MD);
	int GetID()		const;
	int GetWaitingDays()		const;
	void SetWaitingDays(int WD);
	int GetED()		const;			//execution days=2*(Target location/Rover Speed)+Mission Duration
	int GetCD()		const;			//Compeleting days=FD+WD+ED
	void AssignRover(Rover* R);
	Rover* GetRover()		const;
	void DismissRover();
	void SetMissionStatus(MissionStatus status);
	MissionStatus GetMissionStatus ()		const;
	void SetMissionType(MissionType type);
	MissionType GetMissionType()		const;
	~Mission(void);
};

