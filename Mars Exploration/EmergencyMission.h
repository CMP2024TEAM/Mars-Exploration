#pragma once
#include "Mission.h"
class EmergencyMission :
    public Mission
{
public:
    EmergencyMission(int FD, int TL, int MD,int S);
    int GetPriority()const;
    virtual MissionType GetMissionType() const ;
    ~EmergencyMission();
};

