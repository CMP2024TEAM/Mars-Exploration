#pragma once
#include "Event.h"
#include "enums.h"
class FormulationEvent :
    public Event
{
public:
    FormulationEvent(MissionType mType, int ED, int ID, int TLOC, int MDUR, int SIG);
    ~FormulationEvent();
    virtual void Execute(MarsStation*) const;
private:
    MissionType mType;
    int TargetLocation;
    int Duration;
    int Significance;

};

