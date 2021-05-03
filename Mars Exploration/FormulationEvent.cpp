#include "FormulationEvent.h"

FormulationEvent::FormulationEvent(MissionType mType, int ED, int ID, int TLOC, int MDUR, int SIG) : Event(ED,ID)
{
	this->mType = mType;
	TargetLocation = TLOC;
	Duration = MDUR;
	Significance = SIG;
}

FormulationEvent::~FormulationEvent()
{
}

void FormulationEvent::Execute(MarsStation* Mission) const
{
	// TODO : Call Mission Create Mission
}
