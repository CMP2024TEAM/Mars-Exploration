#pragma once
#include "Mission.h"
class PolarMission :
    public Mission
{
public:
    PolarMission(int FD, int TL, int id, int MD, int S);
    virtual MissionType GetMissionType() const;
    ~PolarMission();
};

