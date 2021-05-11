#include "PromotionEvent.h"
#include "Mission.h"

PromotionEvent::PromotionEvent(int ED, int ID):Event(ED,ID)
{
}

PromotionEvent::~PromotionEvent()
{
}

void PromotionEvent::Execute(MarsStation* Station) const
{
	// GetMission From The Station Change Its Type To Emergence Then Add It Again
	Mission* mission = Station->GetMountainouMission(MissionId);
	mission->SetMissionType(MissionType::Emergency);
	Station->AddMission(mission);
}
