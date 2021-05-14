#include "PromotionEvent.h"
#include "Mission.h"
#include "MarsStation.h"

PromotionEvent::PromotionEvent(int ED, int ID):Event(ED,ID)
{
}

PromotionEvent::~PromotionEvent()
{
}

void PromotionEvent::Execute(MarsStation* Station) const
{
	// GetMission From The Station Change Its Type To Emergence Then Add It Again
	Mission* mission;
	Station->GetMountainouMission(mission, MissionId);
	if (mission != nullptr) {
		mission->SetMissionType(MissionType::Emergency);
		Station->AddMission(mission);
	}
}
