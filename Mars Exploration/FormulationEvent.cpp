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
	Mission* mission= new Mission(EventDay,TargetLocation,Significance,Duration,mType);
	Station->AddMission(mission);
}
