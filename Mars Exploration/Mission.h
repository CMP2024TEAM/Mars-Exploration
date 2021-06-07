#pragma once
#include "enums.h"
#include "Rover.h"
class Mission
{
private:
	int ID;
	int FormulationDay;	
	int TargetLocation;		//The distance (in kilometers)
	int Significance;		//representing the importance of the mission
	int MissionDuration;
	int WaitingDays;
	MissionStatus Status;
	Rover* AssignedRover;
	int ED;
	int Get_ED()		const;			//execution days=2*(Target location/Rover Speed)+Mission Duration

public:

	Mission(int FD, int TL,int id ,int MD, int S);
	int GetFormulationDay()		const;
	void SetFormulationDay(int FD);
	int GetTargetLocation ()		const;
	void SetTargetLocation (int TL);
	int GetSignificance()		const;
	void SetSignificance(int S);
	int GetMissionDuration()		const;
	void SetMissionDuration(int MD);
	int GetID()		const;
	void SetID(int id);
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

