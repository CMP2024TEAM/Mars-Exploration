#pragma once
#include "Mission.h"
#include "Rover.h"
#include "DataStructures/PriorityQueue.h"
#include "DataStructures/Queue.h"
#include "Event.h"
#include "FormulationEvent.h"
#include "CancellationEvent.h"
#include "PromotionEvent.h"
#include"UI.h"

class MarsStation
{
private:
	UI* InOut;
	Queue<Event*> EventList;
	PriorityQueue<Rover*> EmergencyRover;
	PriorityQueue<Rover*> MountainousRover;
	PriorityQueue<Rover*> PolarRover;
	PriorityQueue<Mission*> InExceution;
	int AutoP;
	int NumberOfMissionsTheRoverCompletesBeforeCheckup;
	// for while stop condition
	int cInExcecution;
	//for Stats
	int cAutop;
	int cRovers;
	int cMissions;
	int cExcecuteTime;
	int cWaitTime;
public:
	MarsStation() :cInExcecution(0),cRovers(0),cMissions(0),cExcecuteTime(0),cWaitTime(0)
	{
		//this should be allocated outside and then return its pointer
		InOut = new UI;
		InOut->RealAll(this);
	}
	//Initialize Rovar Queues
	void CreateRover(RoverType type, int speed)
	{
		Rover* R = new Rover(type, speed);
		if (type == RoverType::Emergency)
		{
			EmergencyRover.enqueue(MyPair<Rover*, int>(R, speed));
		}
		if (type == RoverType::Mountainous)
		{
			MountainousRover.enqueue(MyPair<Rover*, int>(R, speed));
		}
		if (type == RoverType::Polar)
		{
			PolarRover.enqueue(MyPair<Rover*, int>(R, speed));
		}
		cRovers++;
	}
	//SetCheckupDuration for each rovar tyoe
	void SetCheckupDuration(RoverType type, int Duration)
	{
		if (type == RoverType::Emergency)
			Rover::CheckupDuration[0] = Duration;
		if (type == RoverType::Mountainous)
			Rover::CheckupDuration[1] = Duration;
		if (type == RoverType::Polar)
			Rover::CheckupDuration[2] = Duration;
	}
	//set autopromosion
	void SetAutoP(int apDuration)
	{
		AutoP = (apDuration > 0) ? apDuration : 0;
	}
	void SetNumberOfMissionsTheRoverCompletesBeforeCheckup(int cNum)
	{
		NumberOfMissionsTheRoverCompletesBeforeCheckup = (cNum > 0) ? cNum : 0;
	}
	//create Events
	void CreateFormulationEvent(MissionType mType, int ED, int ID, int TLOC, int MDUR, int SIG)
	{
		Event* E = new FormulationEvent(mType, ED, ID, TLOC, MDUR, SIG);
		EventList.enqueue(E);
	}
	void CreateCancellationEvent(int ED, int ID)
	{
		Event* E = new CancellationEvent(ED, ID);
		EventList.enqueue(E);
	}
	void CreatePromotionEvent(int ED, int ID)
	{
		Event* E = new PromotionEvent(ED, ID);
		EventList.enqueue(E);
	}
	void AssignMissions() // Call this function at the start of every new day
	{
		Mission* Emergent;
		Mission* Mount;
		Mission* Pol;
		Rover* RovEmergent;
		Rover* RovMount;
		Rover* RovPol;
		// Assigning Emergency Missions
		while (!EmergencyWaiting.isEmpty())
		{
			if (!EmergencyRover.isEmpty())
			{
				EmergencyWaiting.dequeue(Emergent);
				EmergencyRover.dequeue(RovEmergent);
				Emergent->AssignRover(RovEmergent);
				InExceution.enqueue(MyPair<Mission*, int>(Emergent, Emergent->GetCD()));
				Emergent->SetMissionStatus(MissionStatus::InExecution);
				cInExcecution++;
			}
			else if (!MountainousRover.isEmpty())
			{
				EmergencyWaiting.dequeue(Emergent);
				MountainousRover.dequeue(RovMount);
				Emergent->AssignRover(RovMount);
				InExceution.enqueue(MyPair<Mission*, int>(Emergent, Emergent->GetCD()));
				Emergent->SetMissionStatus(MissionStatus::InExecution);
				cInExcecution++;
			}
			else if (!PolarRover.isEmpty())
			{
				EmergencyWaiting.dequeue(Emergent);
				EmergencyRover.dequeue(RovPol);
				Emergent->AssignRover(RovPol);
				InExceution.enqueue(MyPair<Mission*, int>(Emergent, Emergent->GetCD()));
				Emergent->SetMissionStatus(MissionStatus::InExecution);
				cInExcecution++;
			}
			else
			{
				break;
			}
		}

		//Assigning Polar Missions

		while (!MountainousWaiting.isEmpty())
		{
			if (!PolarRover.isEmpty())
			{
				EmergencyWaiting.dequeue(Pol);
				EmergencyRover.dequeue(RovPol);
				Pol->AssignRover(RovPol);
				InExceution.enqueue(MyPair<Mission*, int>(Pol, Pol->GetCD()));
				Pol->SetMissionStatus(MissionStatus::InExecution);
				cInExcecution++;
			}
			else
			{
				break;
			}
		}

		//Assigning Mountinous Missions

		while (!MountainousWaiting.isEmpty())
		{
			if (!MountainousRover.isEmpty())
			{
				MountainousWaiting.dequeue(Mount);
				MountainousRover.dequeue(RovMount);
				Mount->AssignRover(RovMount);
				InExceution.enqueue(MyPair<Mission*, int>(Mount, Mount->GetCD()));
				Mount->SetMissionStatus(MissionStatus::InExecution);
				cInExcecution++;

			}
			else if (!EmergencyRover.isEmpty())
			{
				MountainousWaiting.dequeue(Mount);
				EmergencyRover.dequeue(RovEmergent);
				Mount->AssignRover(RovEmergent);
				InExceution.enqueue(MyPair<Mission*, int>(Mount, Mount->GetCD()));
				Mount->SetMissionStatus(MissionStatus::InExecution);
				cInExcecution++;
			}
			else
			{
				break;
			}
		}


	}
	~MarsStation()
	{
		Rover* R;
		Event* E;
		Mission* M;
		delete InOut;
		while (EmergencyRover.dequeue(R))
		{
			delete R;
		}
		while (MountainousRover.dequeue(R))
		{
			delete R;
		}
		while (PolarRover.dequeue(R))
		{
			delete R;
		}
		while (EventList.dequeue(E))
		{
			delete E;
		}
		while (InExceution.dequeue(M))
		{
			delete M;
		}
	}



};
