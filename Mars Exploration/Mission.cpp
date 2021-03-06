#include "Mission.h"
#include "cmath"

Mission::Mission(int FD, int TL,int id, int MD, int S)
{
	SetFormulationDay(FD);
	SetTargetLocation(TL);
	SetMissionDuration(MD);
	SetSignificance(S);
	Status = MissionStatus::Waiting;
	AssignedRover = nullptr;
	ID=0;
	SetWaitingDays(0);
	SetID(id);
}

int Mission::GetFormulationDay()			const
{
	return FormulationDay;
}

void Mission::SetFormulationDay(int FD)
{
	FormulationDay = FD > 0 ? FD : 1;
}

int Mission::GetTargetLocation()		const
{
	return TargetLocation;
}

void Mission::SetTargetLocation(int TL)
{
	TargetLocation = TL > 0 ? TL : 1;
}

int Mission::GetSignificance()		const
{
	return Significance;
}

void Mission::SetSignificance(int S)
{
	Significance = S > 0 ? S : 1;
}

int Mission::GetMissionDuration()		const
{
	return MissionDuration;
}

void Mission::SetMissionDuration(int MD)
{
	MissionDuration = MD > 0 ? MD : 1;
}

int Mission::GetID()		const
{
	return ID;
}

void Mission::SetID(int id)
{
	ID = id > 0 ? id : 0;
}

int Mission::GetWaitingDays()		const
{
	return WaitingDays;
}

void Mission::SetWaitingDays(int WD)
{
	WaitingDays = WD >= 0 ? WD : 0;
}

int Mission::Get_ED() const
{
	if (AssignedRover == nullptr)
		return 0;
	else
		return ceil(2 * ((float(TargetLocation) / AssignedRover->getSpeed()) / 25) + MissionDuration);
}

void Mission::AssignRover(Rover* R)
{
	AssignedRover = R;
}

void Mission::DismissRover()
{
	AssignedRover = nullptr;
}

Rover* Mission::GetRover()		const
{
	return AssignedRover;
}

int	Mission::GetCD()		const
{
	return FormulationDay + ED + WaitingDays;
}

void Mission::SetMissionStatus(MissionStatus status)
{
	Status = status;
}

MissionStatus Mission::GetMissionStatus()		const
{
	return Status;
}

void Mission::SetED()
{
	ED = Get_ED();
}
int Mission::GetED() const
{
	return ED;
}

Mission::~Mission(void)
{
}
