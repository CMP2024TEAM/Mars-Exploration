#include "EmergencyMission.h"

EmergencyMission::EmergencyMission(int FD, int TL, int MD, int S) :Mission(FD, TL, MD,S)
{

}

int EmergencyMission::GetPriority() const
{
	return(GetFormulationDay() * -1 + GetSignificance() * 5 + int(GetTargetLocation() * -0.01) + GetMissionDuration() * -2);
}

MissionType EmergencyMission::GetMissionType()	const
{
	return MissionType::Emergency;
}

EmergencyMission::~EmergencyMission()
{

}