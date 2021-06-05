#include "PolarMission.h"

PolarMission::PolarMission(int FD, int TL, int MD, int S) :Mission(FD, TL, MD, S)
{

}

MissionType PolarMission::GetMissionType() const
{
	return MissionType::Polar;
}

PolarMission::~PolarMission()
{

}