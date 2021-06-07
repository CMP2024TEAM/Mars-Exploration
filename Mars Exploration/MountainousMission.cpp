#include "MountainousMission.h"

MountainousMission::MountainousMission(int FD, int TL,int id, int MD, int S) :Mission(FD,TL,id,MD,S)
{

}

MissionType MountainousMission::GetMissionType() const
{
	return MissionType::Mountainous;
}

MountainousMission::~MountainousMission()
{

}