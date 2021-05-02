#pragma once
#include "enums.h"
#include "rover.h"
class Mission
{
private:
	int FormulationDay;	
	int TargetLocation;		//The distance (in kilometers)
	int MissionLocation;		
	int Significance;		//representing the importance of the mission
	static int ID;
	int MissionDuration;
	int WaitingDays;
	MissionType Type;
	MissionStatus Status;
	rover* Rover;
	
public:

	Mission(int FD,int TL,int ML,int S,int MD,int WD,MissionType T);
	int GetFormulationDay()		const;
	void SetFormulationDay(int FD);
	int GetTargetLocation ()		const;
	void SetTargetLocation (int TL);
	int GetMissionLocation ()		const;
	void SetMissionLocation (int ML);
	int GetSignificance()		const;
	void SetSignificance(int S);
	int GetMissionDuration()		const;
	void SetMissionDuration(int MD);
	int GetID()		const;
	int GetWaitingDays()		const;
	void SetWaitingDays(int WD);
	int GetED()		const;			//execution days=2*(Target location/Rover Speed)+Mission Duration
	int GetCD()		const;			//Compeleting days=FD+WD+ED
	void AssingRover(rover* R);
	rover* GetRover()		const;
	void DismissRover();
	void SetMissionStatus(MissionStatus status);
	MissionStatus GetMissionStatus ()		const;
	void SetMissionType(MissionType type);
	MissionType GetMissionType()		const;
	~Mission(void);
};

