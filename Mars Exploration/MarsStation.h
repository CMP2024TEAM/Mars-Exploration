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
	PriorityQueue<Mission*> WaitingEmergencyMissions;
	Queue<Mission*> WaitingPolarMissions;
	Queue<Mission*> WaitingMountainousMissions;
	PriorityQueue<Rover*> EmergencyRover;
	PriorityQueue<Rover*> MountainousRover;
	PriorityQueue<Rover*> PolarRover;
	PriorityQueue<Mission*> InExceution;
	int AutoP;
	int NumberOfMissionsTheRoverCompletesBeforeCheckup;
	// For while stop condition
	int cInExcecution;
	// Counts
	int WaitingEmergencyMissionCount;
	int WaitingMountainousMissionCount;
	int WaitingPolarMissionCount;
	// For Stats
	int cAutop;
	int cRovers;
	int cMissions;
	int cExcecuteTime;
	int cWaitTime;
public:
	// Constructor
	MarsStation() :cInExcecution(0),cRovers(0),cMissions(0),cExcecuteTime(0),cWaitTime(0)
	{
		//this should be allocated outside and then return its pointer
		InOut = new UI(OutputType::Silent);
		InOut->RealAll(this);
		WaitingEmergencyMissionCount = 0;
		WaitingMountainousMissionCount = 0;
		WaitingPolarMissionCount = 0;
	}
	// Initialize Rovar Queues
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
	// SetCheckupDuration for each rovar tyoe
	void SetCheckupDuration(RoverType type, int Duration)
	{
		if (type == RoverType::Emergency)
			Rover::CheckupDuration[0] = Duration;
		if (type == RoverType::Mountainous)
			Rover::CheckupDuration[1] = Duration;
		if (type == RoverType::Polar)
			Rover::CheckupDuration[2] = Duration;
	}
	// Set autopromosion
	void SetAutoP(int apDuration)
	{
		AutoP = (apDuration > 0) ? apDuration : 0;
	}
	void SetNumberOfMissionsTheRoverCompletesBeforeCheckup(int cNum)
	{
		NumberOfMissionsTheRoverCompletesBeforeCheckup = (cNum > 0) ? cNum : 0;
	}
	// Create Events
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
	// Call this function at the start of every new day
	void AssignMissions() 
	{
		Mission* Emergent;
		Mission* Mount;
		Mission* Pol;
		Rover* RovEmergent;
		Rover* RovMount;
		Rover* RovPol;
		// Assigning Emergency Missions
		while (!WaitingEmergencyMissions.isEmpty())
		{
			if (!EmergencyRover.isEmpty())
			{
				WaitingEmergencyMissions.dequeue(Emergent);
				EmergencyRover.dequeue(RovEmergent);
				Emergent->AssignRover(RovEmergent);
				InExceution.enqueue(MyPair<Mission*, int>(Emergent, Emergent->GetCD()));
				Emergent->SetMissionStatus(MissionStatus::InExecution);
				cInExcecution++;
			}
			else if (!MountainousRover.isEmpty())
			{
				WaitingEmergencyMissions.dequeue(Emergent);
				MountainousRover.dequeue(RovMount);
				Emergent->AssignRover(RovMount);
				InExceution.enqueue(MyPair<Mission*, int>(Emergent, Emergent->GetCD()));
				Emergent->SetMissionStatus(MissionStatus::InExecution);
				cInExcecution++;
			}
			else if (!PolarRover.isEmpty())
			{
				WaitingEmergencyMissions.dequeue(Emergent);
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

		while (!WaitingPolarMissions.isEmpty())
		{
			if (!PolarRover.isEmpty())
			{
				WaitingPolarMissions.dequeue(Pol);
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

		while (!WaitingMountainousMissions.isEmpty())
		{
			if (!MountainousRover.isEmpty())
			{
				WaitingMountainousMissions.dequeue(Mount);
				MountainousRover.dequeue(RovMount);
				Mount->AssignRover(RovMount);
				InExceution.enqueue(MyPair<Mission*, int>(Mount, Mount->GetCD()));
				Mount->SetMissionStatus(MissionStatus::InExecution);
				cInExcecution++;

			}
			else if (!EmergencyRover.isEmpty())
			{
				WaitingMountainousMissions.dequeue(Mount);
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
	// Add Mission To its corresponding list
	void AddMission(Mission* mission)
	{
		// Check Type Of Mission Then Add To Corrersponding List
		switch (mission->GetMissionType())
		{
		case MissionType::Emergency :
			WaitingEmergencyMissions.enqueue(MyPair<Mission*, int>(mission, mission->GetSignificance()));
			WaitingEmergencyMissionCount++;
			break;
		case MissionType::Polar:
			WaitingPolarMissions.enqueue(mission);
			WaitingPolarMissionCount++;
			break;
		case MissionType::Mountainous:
			WaitingMountainousMissions.enqueue(mission);
			WaitingMountainousMissionCount++;
			break;
		default:
			break;
		}
	}
	// Get Waiting Mountainous Mission With Certain ID (USed In Events Execution)
	bool GetMountainouMission(Mission*& mission, int ID)
	{
		mission = nullptr;
		Mission* m;
		Queue<Mission*> temp;
		// Search For A Mission With This ID
		while (WaitingMountainousMissions.dequeue(m))
		{
			if (m->GetID() == ID)
			{
				mission = m;
			}
			else
			{
				temp.enqueue(m);
			}
		}
		// Return List As It Was
		while (temp.dequeue(m))
		{
			WaitingMountainousMissions.enqueue(m);
		}
		// Not Found
		if (mission == nullptr)
		{
			return false;
		}
		// If Found Decrease Count And Return True 
		WaitingMountainousMissionCount--;
		return true;
	}
	// Destructor
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
