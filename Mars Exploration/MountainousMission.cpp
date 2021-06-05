#include "MountainousMission.h"

MountainousMission::MountainousMission(int FD, int TL, int MD, int S) :Mission(FD,TL,MD,S)
{

}

MissionType MountainousMission::GetMissionType() const
{
	return MissionType::Mountainous;
}

MountainousMission::~MountainousMission()
{

}