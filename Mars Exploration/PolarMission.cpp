#include "PolarMission.h"

PolarMission::PolarMission(int FD, int TL, int MD) :Mission(FD, TL, MD)
{

}

MissionType PolarMission::GetMissionType() const
{
	return MissionType::Polar;
}

PolarMission::~PolarMission()
{

}