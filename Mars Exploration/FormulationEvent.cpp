#include "FormulationEvent.h"
#include "Mission.h"
#include "MarsStation.h"

FormulationEvent::FormulationEvent(MissionType mType, int ED, int ID, int TLOC, int MDUR, int SIG) : Event(ED,ID)
{
	this->mType = mType;
	TargetLocation = TLOC;
	Duration = MDUR;
	Significance = SIG;
}

FormulationEvent::~FormulationEvent()
{
}

void FormulationEvent::Execute(MarsStation* Station) const
{
	// Create Mission And Add it To The Station
	Mission* mission;
	switch (mType)
	{
	case MissionType::Emergency:
		mission = new EmergencyMission(EventDay, TargetLocation, MissionId, Significance, Duration);
		break;
	case MissionType::Mountainous:
		mission = new MountainousMission(EventDay, TargetLocation, MissionId, Significance, Duration);
		break;
	case MissionType::Polar:
		mission = new PolarMission(EventDay, TargetLocation, MissionId, Significance, Duration);
		break;
	default:
		throw("Unknown Mission Type");
		break;
	}
	Station->AddMission(mission);
}
