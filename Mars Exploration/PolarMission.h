#pragma once
#include "Mission.h"
class PolarMission :
    public Mission
{
public:
    PolarMission(int FD, int TL, int MD);
    virtual MissionType GetMissionType() const;
    ~PolarMission();
};

