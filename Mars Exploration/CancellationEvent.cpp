#include "CancellationEvent.h"
#include "Mission.h"
#include "MarsStation.h"


CancellationEvent::CancellationEvent(int ED, int ID) : Event(ED,ID)
{
}

CancellationEvent::~CancellationEvent()
{
}

void CancellationEvent::Execute(MarsStation* Station) const
{
	// Get Mission From Station Then Delete it
	Mission* mission;
	Station->GetMountainouMission(mission, MissionId);
	if (mission != nullptr)
		delete mission;
}
