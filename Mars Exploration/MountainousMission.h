#pragma once
#include "Mission.h"
class MountainousMission :
    public Mission
{
public:
    MountainousMission(int FD, int TL,int id,int MD, int S);
    virtual MissionType GetMissionType() const;
    ~MountainousMission();
};

