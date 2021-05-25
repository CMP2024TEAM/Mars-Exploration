#include "Mission.h"

int Mission::ID = 0;

Mission::Mission(int FD, int TL, int S, int MD, MissionType T)
{
	SetFormulationDay(FD);
	SetTargetLocation(TL);
	SetSignificance(S);
	SetMissionDuration(MD);
	Type = T;
	Status = MissionStatus::Waiting;
	AssignedRover = nullptr;
	ID++;
	Id = ID;
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
	return Id;
}

int Mission::GetWaitingDays()		const
{
	return WaitingDays;
}

void Mission::SetWaitingDays(int WD)
{
	WaitingDays = WD > 0 ? WD : 1;
}

int Mission::Get_ED()		const
{
	if (AssignedRover == nullptr)
		return 0;
	else
		return 2 * ((TargetLocation / AssignedRover->getSpeed()) / 24) + MissionDuration;
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

void Mission::SetMissionType(MissionType T)
{
	Type = T;
}

MissionType Mission::GetMissionType()		const
{
	return Type;
}

void Mission::SetED()
{
	ED = Get_ED();
}
int Mission::GetED() const
{
	return ED;
}
int Mission::GetPriority() const
{
	if (Type != MissionType::Emergency)
		return 0;
	else
	{
		return(FormulationDay * -1 + Significance * 5 + int(TargetLocation * -0.01) + MissionDuration * -2);
	}
}
Mission::~Mission(void)
{
}
