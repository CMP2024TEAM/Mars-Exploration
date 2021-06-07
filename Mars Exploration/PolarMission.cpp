#include "PolarMission.h"

PolarMission::PolarMission(int FD, int TL, int id, int MD, int S) :Mission(FD, TL,id, MD, S)
{

}

MissionType PolarMission::GetMissionType() const
{
	return MissionType::Polar;
}

PolarMission::~PolarMission()
{

}