#include "Mission.h"
#include "enums.h"
#include "rover.h"

int Mission::ID=0;

Mission::Mission(int FD,int TL,int ML,int S,int MD,int WD,MissionType T)
{
	SetFormulationDay(FD);
	SetTargetLocation(TL);
	SetMissionLocation(ML);
	SetSignificance(S);
	SetMissionDuration(MD);
	SetWaitingDays(WD);
	Type=T;
	Status=MissionStatus::Waiting;
	Rover=nullptr;
	ID++;
}

int Mission::GetFormulationDay()			const
{
	return FormulationDay;
}

void Mission::SetFormulationDay(int FD)
{
	FormulationDay=FD>0?FD:1;
}

int Mission::GetTargetLocation()		const
{
	return TargetLocation;
}

void Mission::SetTargetLocation(int TL)
{
	TargetLocation=TL>0?TL:1;
}

int Mission::GetMissionLocation()		const
{
	return MissionLocation;
}

void Mission::SetMissionLocation(int ML)
{
	MissionLocation=ML>0?ML:1;
}

int Mission::GetSignificance()		const
{
	return Significance;
}

void Mission::SetSignificance(int S)
{
	Significance=S>0?S:1;
}

int Mission::GetMissionDuration()		const
{
	return MissionDuration;
}

void Mission::SetMissionDuration(int MD)
{
	MissionDuration=MD>0?MD:1;
}

int Mission::GetID()		const
{
	return ID;
}

int Mission::GetWaitingDays()		const
{
	return WaitingDays;
}

void Mission::SetWaitingDays(int WD)
{
	WaitingDays=WD>0?WD:1;
}

int Mission::GetED()		const
{
	if (Rover==nullptr)
		return 0;
	else
		return 2*(TargetLocation/Rover->getSpeed())+MissionDuration;
}

void Mission::AssingRover(rover* R)
{
	Rover=R;
}

void Mission::DismissRover()
{
	Rover=nullptr;
}

rover* Mission::GetRover()		const
{
	return Rover;
}

int	Mission::GetCD()		const
{
	return FormulationDay+GetED()+WaitingDays;
}

void Mission::SetMissionStatus(MissionStatus status)
{
	Status=status;
}

MissionStatus Mission::GetMissionStatus()		const
{
	return Status;
}

void Mission::SetMissionType(MissionType T)
{
	Type=T;
}

MissionType Mission::GetMissionType()		const
{
	return Type;
}

Mission::~Mission(void)
{
}
