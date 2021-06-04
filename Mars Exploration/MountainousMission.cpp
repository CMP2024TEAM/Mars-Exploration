#include "MountainousMission.h"

MountainousMission::MountainousMission(int FD, int TL, int MD) :Mission(FD,TL,MD)
{

}

MissionType MountainousMission::GetMissionType() const
{
	return MissionType::Mountainous;
}

MountainousMission::~MountainousMission()
{

}