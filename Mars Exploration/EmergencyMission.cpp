#include "EmergencyMission.h"

EmergencyMission::EmergencyMission(int FD, int TL, int MD, int S) :Mission(FD, TL, MD)
{
	SetSignificance(S);
}

int EmergencyMission::GetSignificance()		const
{
	return Significance;
}

void EmergencyMission::SetSignificance(int S)
{
	Significance = S > 0 ? S : 1;
}

int EmergencyMission::GetPriority() const
{
	return(GetFormulationDay() * -1 + Significance * 5 + int(GetTargetLocation() * -0.01) + GetMissionDuration() * -2);
}

MissionType EmergencyMission::GetMissionType()	const
{
	return MissionType::Emergency;
}

EmergencyMission::~EmergencyMission()
{

}