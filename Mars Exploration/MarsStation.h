#pragma once
#include "Mission.h"
#include "Rover.h"
#include "DataStructures/PriorityQueue.h"
#include "DataStructures/Queue.h"
#include "DataStructures/Stack.h"
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
	Queue<Rover*> EmergencyRoverCheckUp;
	Queue<Rover*> MountinousRoverCheckUp;
	Queue<Rover*> PolarRoverCheckUp;
	Stack<Mission*> CompletedMissions;
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
	int Day;
public:
	// Constructor
	MarsStation() :cInExcecution(0), cRovers(0), cMissions(0), cExcecuteTime(0), cWaitTime(0), cAutop(0), Day(1)
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
		case MissionType::Emergency:
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

	//start a new day
	void IncreaseDay()
	{
		Day++;
	}

	int GetCurrentDay()
	{
		return Day;
	}

	//Execute events stored in Queue
	void ExecuteEvent()
	{
		Event* execute;
		//loop on the events queue if it is the time to execute event then do it else end the function
		while (true)	
		{
			execute = nullptr;
			EventList.peekFront(execute);			
			//remove this event from the queue after execute it
			if (execute&&execute->GetEventDay() == Day)
			{
				execute->Execute(this);
				EventList.dequeue(execute);
			}
			else
				break;
		}
	}

	//Move rovers in check up to be available to assign a mission 
	void MoveCheckUpToAvail()
	{
		Rover* Erover = nullptr;
		Rover* Mrover = nullptr;
		Rover* Prover = nullptr;
		//check the first rover in each list if it is the day to move it then do it else end the function
		while (true)
		{
			EmergencyRoverCheckUp.peekFront(Erover);
			MountinousRoverCheckUp.peekFront(Mrover);
			PolarRoverCheckUp.peekFront(Prover);
			//condition to exit the loop
			//exit if there is no rover in check up or there is no rover has finished its check up duration
			if ((!Erover&&!Mrover&&!Prover)||Erover->getAvailableAt() > Day && Mrover->getAvailableAt() > Day && Prover->getAvailableAt() > Day)
				break;
			if (Erover&&Erover->getAvailableAt() == Day)
			{
				EmergencyRoverCheckUp.dequeue(Erover);
				EmergencyRover.enqueue(MyPair<Rover*, int>(Erover, Erover->getSpeed()));
			}
			if (Mrover&&Mrover->getAvailableAt() == Day)
			{
				MountinousRoverCheckUp.dequeue(Mrover);
				MountainousRover.enqueue(MyPair<Rover*, int>(Mrover, Mrover->getSpeed()));
			}
			if (Prover&&Prover->getAvailableAt() == Day)
			{
				PolarRoverCheckUp.dequeue(Prover);
				PolarRover.enqueue(MyPair<Rover*, int>(Prover, Prover->getSpeed()));
			}
		}
	}

	//remove the link between the mission and the rover
	//check if this rover needs to have a checkup or not 
	void DismissMissions(Mission* M)
	{
		Rover* ReturnRover;
		ReturnRover = M->GetRover();
		//remove the link between the mission and the rover
		M->DismissRover();
		ReturnRover->increaseCompletedMissions();
		//check if this rover needs to have a check up or not
		if (NumberOfMissionsTheRoverCompletesBeforeCheckup <= ReturnRover->getCompletedMissions())
		{
			ReturnRover->setStatus(RoverStatus::InCheckUp);
			//detemine the type of the rover and put it in the appropiate queue
			switch (ReturnRover->getType())
			{
			case RoverType::Emergency:EmergencyRoverCheckUp.enqueue(ReturnRover); break;
			case RoverType::Mountainous:MountinousRoverCheckUp.enqueue(ReturnRover); break;
			case RoverType::Polar:PolarRoverCheckUp.enqueue(ReturnRover); break;
			}
		}
		//the rover do not need to have a check up
		else
		{
			ReturnRover->setStatus(RoverStatus::Available);
			//detemine the type of the rover and put it in the appropiate queue
			switch (ReturnRover->getType())
			{
			case RoverType::Emergency:EmergencyRover.enqueue(MyPair<Rover*, int>(ReturnRover, ReturnRover->getSpeed())); break;
			case RoverType::Mountainous:MountainousRover.enqueue(MyPair<Rover*, int>(ReturnRover, ReturnRover->getSpeed())); break;
			case RoverType::Polar:PolarRover.enqueue(MyPair<Rover*, int>(ReturnRover, ReturnRover->getSpeed())); break;
			}
		}
	}

	//remove the mission from in-exectution queue then add it to the complete missions after doing its fulfill mission requirements
	void MoveInExcecutiontoComplete()
	{
		Mission* M;
		while (true)
		{
			M = nullptr;
			InExceution.peekFront(M);
			//check if there is any in execution missions, if any,check if this day is the day on which a certain mission will complete its requirements
			if (M&&M->GetCD() == Day)
			{
				InExceution.dequeue(M);
				cInExcecution--;
				M->SetMissionStatus(MissionStatus::Completed);
				CompletedMissions.push(M);
				//remove the link between the mission and the rover. Put the rover in the appropiate list
				DismissMissions(M);
			}
			else
				break;
		}
	}

	//check if there is any mountainous mission that has been waiting more than the auto promotion duration
	//if there is, remove it from the queue of the mountainous then enqeue it in the emergency waiting missions
	void CheckUpAutoP()
	{
		Mission* M;
		while (true)
		{
			M = nullptr;
			WaitingMountainousMissions.peekFront(M);
			//check if there is any mountainous mission , if any , compare the waiting days with the autoP
			if (M&&M->GetWaitingDays() > AutoP)
			{
				WaitingMountainousMissions.dequeue(M);
				M->SetMissionType(MissionType::Emergency);
				WaitingEmergencyMissions.enqueue(MyPair<Mission*, int>(M, M->GetSignificance()));
				cAutop++;	//used to calculate the percentage of automatically-promoted missions (relative to the total number of mountainous missions)
			}
			else
				break;
		}
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
		while (EmergencyRoverCheckUp.dequeue(R))
		{
			delete R;
		}
		while (MountinousRoverCheckUp.dequeue(R))
		{
			delete R;
		}	
		while (PolarRoverCheckUp.dequeue(R))
		{
			delete R;
		}
		while (WaitingEmergencyMissions.dequeue(M))
		{
			delete M;
		}
		while (WaitingPolarMissions.dequeue(M))
		{
			delete M;
		}
		while (WaitingMountainousMissions.dequeue(M))
		{
			delete M;
		}
		while (CompletedMissions.pop(M))
		{
			delete M;
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
