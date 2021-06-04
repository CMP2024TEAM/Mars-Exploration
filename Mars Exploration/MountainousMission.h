#pragma once
#include "Mission.h"
class MountainousMission :
    public Mission
{
public:
    MountainousMission(int FD, int TL, int MD);
    virtual MissionType GetMissionType() const;
    ~MountainousMission();
};

