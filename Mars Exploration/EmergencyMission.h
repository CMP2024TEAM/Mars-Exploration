#pragma once
#include "Mission.h"
class EmergencyMission :
    public Mission
{
private:
    int Significance;		//representing the importance of the mission
public:
    EmergencyMission(int FD, int TL, int MD,int S);
    int GetSignificance()		const;
    void SetSignificance(int S);
    int GetPriority()const;
    virtual MissionType GetMissionType() const ;
    ~EmergencyMission();
};

