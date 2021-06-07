#include "EmergencyMission.h"

EmergencyMission::EmergencyMission(int FD, int TL, int id, int MD, int S) :Mission(FD, TL,id, MD,S)
{

}

EmergencyMission::EmergencyMission(Mission* M):Mission(M->GetFormulationDay(),M->GetTargetLocation(),M->GetID(),M->GetMissionDuration(),M->GetSignificance())
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