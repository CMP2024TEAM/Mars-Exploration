#pragma once
#include "Mission.h"
#include "MountainousMission.h"
class EmergencyMission :
    public Mission
{
public:
    EmergencyMission(int FD, int TL, int id, int MD, int S);
    //copy constructor to be used in Auto P process
    EmergencyMission(Mission* M);
    int GetPriority()const;
    virtual MissionType GetMissionType() const ;
    ~EmergencyMission();
};

