#include "MarsStation.h"

// Constructor
MarsStation::MarsStation(UI* tInOut) :cInExecution(0), cEmergencyMissions(0), cMountainousMissions(0), cPolarMissions(0), cEmergencyRovers(0), cPolarRovers(0), cMountainousRovers(0), cExcecuteTime(0), cWaitTime(0), cAutop(0), Day(1)
{
	//this should be allocated outside and then return its pointer
	InOut = tInOut;
	InOut->ReadAll(this);
	WaitingEmergencyMissionCount = 0;
	WaitingMountainousMissionCount = 0;
	WaitingPolarMissionCount = 0;
	/* initialize random seed: */
	//srand(time(NULL));
	srand(10);
	cInCheckUp = 0;			
	cCompletedMissions = 0;
}

void MarsStation::CreateRover(RoverType type, int speed)
{
	Rover* R = new Rover(type, speed);
	if (type == RoverType::Emergency)
	{
		EmergencyRovers.enqueue(MyPair<Rover*, int>(R, speed));
		cEmergencyRovers++;
	}
	if (type == RoverType::Mountainous)
	{
		MountainousRovers.enqueue(MyPair<Rover*, int>(R, speed));
		cMountainousRovers++;
	}
	if (type == RoverType::Polar)
	{
		PolarRovers.enqueue(MyPair<Rover*, int>(R, speed));
		cPolarRovers++;
	}
}

// SetCheckupDuration for each rovar tyoe

void MarsStation::SetCheckupDuration(RoverType type, int Duration)
{
	if (type == RoverType::Emergency)
		Rover::CheckupDuration[0] = Duration;
	if (type == RoverType::Mountainous)
		Rover::CheckupDuration[1] = Duration;
	if (type == RoverType::Polar)
		Rover::CheckupDuration[2] = Duration;
}

// Set autopromosion

void MarsStation::SetAutoP(int apDuration)
{
	AutoP = (apDuration > 0) ? apDuration : 0;
}

void MarsStation::SetMissionsBeforeCheckup(int cNum)
{
	MissionsBeforeCheckup = (cNum > 0) ? cNum : 0;
}

// Create Events

void MarsStation::CreateFormulationEvent(MissionType mType, int ED, int ID, int TLOC, int MDUR, int SIG)
{
	Event* E = new FormulationEvent(mType, ED, ID, TLOC, MDUR, SIG);
	EventList.enqueue(E);
}

void MarsStation::CreateCancellationEvent(int ED, int ID)
{
	Event* E = new CancellationEvent(ED, ID);
	EventList.enqueue(E);
}

void MarsStation::CreatePromotionEvent(int ED, int ID)
{
	Event* E = new PromotionEvent(ED, ID);
	EventList.enqueue(E);
}

// Call this function at the start of every new day
void MarsStation::AssignMissions()
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
		if (!EmergencyRovers.isEmpty())
		{
			WaitingEmergencyMissions.dequeue(Emergent);
			EmergencyRovers.dequeue(RovEmergent);
			Emergent->AssignRover(RovEmergent);
			InExecutionMissions.enqueue(MyPair<Mission*, int>(Emergent, -1 * Emergent->GetCD()));
			Emergent->SetMissionStatus(MissionStatus::InExecution);
			Emergent->SetWaitingDays(Day - Emergent->GetFormulationDay());
			cEmergencyRovers--;
			cEmergencyMissions--;
			cInExecution++;
			WaitingEmergencyMissionCount--;
		}
		else if (!MountainousRovers.isEmpty())
		{
			WaitingEmergencyMissions.dequeue(Emergent);
			MountainousRovers.dequeue(RovMount);
			Emergent->AssignRover(RovMount);
			InExecutionMissions.enqueue(MyPair<Mission*, int>(Emergent, -1 * Emergent->GetCD()));
			Emergent->SetMissionStatus(MissionStatus::InExecution);
			Emergent->SetWaitingDays(Day - Emergent->GetFormulationDay());
			cMountainousRovers--;
			cEmergencyMissions--;
			cInExecution++;
			WaitingEmergencyMissionCount--;
		}
		else if (!PolarRovers.isEmpty())
		{
			WaitingEmergencyMissions.dequeue(Emergent);
			PolarRovers.dequeue(RovPol);
			Emergent->AssignRover(RovPol);
			InExecutionMissions.enqueue(MyPair<Mission*, int>(Emergent, -1 * Emergent->GetCD()));
			Emergent->SetMissionStatus(MissionStatus::InExecution);
			Emergent->SetWaitingDays(Day - Emergent->GetFormulationDay());
			cPolarRovers--;
			cEmergencyMissions--;
			cInExecution++;
			WaitingEmergencyMissionCount--;
		}
		else
		{
			break;
		}
	}

	//Assigning Polar Missions

	while (!WaitingPolarMissions.isEmpty())
	{
		if (!PolarRovers.isEmpty())
		{
			WaitingPolarMissions.dequeue(Pol);
			PolarRovers.dequeue(RovPol);
			Pol->AssignRover(RovPol);
			InExecutionMissions.enqueue(MyPair<Mission*, int>(Pol, -1 * Pol->GetCD()));
			Pol->SetMissionStatus(MissionStatus::InExecution);
			Pol->SetWaitingDays(Day - Pol->GetFormulationDay());
			cPolarRovers--;
			cPolarMissions--;
			cInExecution++;
			WaitingPolarMissionCount--;
		}
		else
		{
			break;
		}
	}

	//Assigning Mountinous Missions

	while (!WaitingMountainousMissions.isEmpty())
	{
		if (!MountainousRovers.isEmpty())
		{
			WaitingMountainousMissions.dequeue(Mount);
			MountainousRovers.dequeue(RovMount);
			Mount->AssignRover(RovMount);
			InExecutionMissions.enqueue(MyPair<Mission*, int>(Mount, -1 * Mount->GetCD()));
			Mount->SetMissionStatus(MissionStatus::InExecution);
			Mount->SetWaitingDays(Day - Mount->GetFormulationDay());
			cMountainousRovers--;
			cMountainousMissions--;
			cInExecution++;
			WaitingMountainousMissionCount--;

		}
		else if (!EmergencyRovers.isEmpty())
		{
			WaitingMountainousMissions.dequeue(Mount);
			EmergencyRovers.dequeue(RovEmergent);
			Mount->AssignRover(RovEmergent);
			InExecutionMissions.enqueue(MyPair<Mission*, int>(Mount, -1 * Mount->GetCD()));
			Mount->SetMissionStatus(MissionStatus::InExecution);
			Mount->SetWaitingDays(Day - Mount->GetFormulationDay());
			cEmergencyRovers--;
			cMountainousMissions--;
			cInExecution++;
			WaitingMountainousMissionCount--;
		}
		else
		{
			break;
		}
	}
}

// Add Mission To its corresponding list
void MarsStation::AddMission(Mission* mission)
{
	// Check Type Of Mission Then Add To Corrersponding List
	switch (mission->GetMissionType())
	{
	case MissionType::Emergency:
		WaitingEmergencyMissions.enqueue(MyPair<Mission*, int>(mission, mission->GetSignificance()));
		WaitingEmergencyMissionCount++;
		cEmergencyMissions++;
		break;
	case MissionType::Polar:
		WaitingPolarMissions.enqueue(mission);
		WaitingPolarMissionCount++;
		cPolarMissions++;
		break;
	case MissionType::Mountainous:
		WaitingMountainousMissions.enqueue(mission);
		WaitingMountainousMissionCount++;
		cMountainousMissions++;
		break;
	default:
		break;
	}
}

// Get Waiting Mountainous Mission With Certain ID (USed In Events Execution)
bool MarsStation::GetMountainouMission(Mission*& mission, int ID)
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
	cMountainousMissions--;
	return true;
}

// Start a new day
void MarsStation::IncreaseDay()
{
	Day++;
}

// Get Current day
int MarsStation::GetCurrentDay()
{
	return Day;
}

// Execute events stored in Queue
void MarsStation::ExecuteEvent()
{
	Event* execute;
	//loop on the events queue if it is the time to execute event then do it else end the function
	while (true)
	{
		execute = nullptr;
		EventList.peekFront(execute);
		//remove this event from the queue after execute it
		if (execute && execute->GetEventDay() == Day)
		{
			execute->Execute(this);
			EventList.dequeue(execute);
			delete execute;
		}
		else
			break;
	}
}

// Move rovers in check up to be available to assign a mission 
void MarsStation::MoveCheckUpToAvail()
{
	Rover* Erover;
	Rover* Mrover;
	Rover* Prover;
	bool Eflag = true, Mflag = true, Pflag = true;		//determine if the Available day of the rover is less than the current day
	//check the first rover in each list if it is the day to move it then do it else end the function
	while (Eflag || Mflag || Pflag)
	{
		Erover = nullptr;
		Mrover = nullptr;
		Prover = nullptr;
		EmergencyRoversCheckUp.peekFront(Erover);
		MountinousRoverCheckUp.peekFront(Mrover);
		PolarRoversCheckUp.peekFront(Prover);
		//condition to exit the loop
		//exit if there is no rover in check up
		if (!Erover && !Mrover && !Prover)
		{
			break;
		}
		//determine if there is a queue that its rover's available day < current Day
		if (Eflag && Erover && Erover->getAvailableAt() < Day)
		{
			Eflag = false;
		}
		else if (Erover)
		{
			EmergencyRoversCheckUp.dequeue(Erover);
			EmergencyRovers.enqueue(MyPair<Rover*, int>(Erover, Erover->getSpeed()));
			cInCheckUp--;
			cEmergencyRovers++;
		}
		else 
		{
			Eflag = false;
		}
		if (Mflag && Mrover && Mrover->getAvailableAt() < Day)
			Mflag = false;
		else if (Mrover)
		{
			MountinousRoverCheckUp.dequeue(Erover);
			MountainousRovers.enqueue(MyPair<Rover*, int>(Erover, Erover->getSpeed()));
			cInCheckUp--;
			cMountainousRovers++;
		}
		else
		{
			Mflag = false;
		}
		if (Pflag && Prover && Prover->getAvailableAt() < Day)
		{
			Pflag = false;
		}
		else if (Prover)
		{
			PolarRoversCheckUp.dequeue(Erover);
			PolarRovers.enqueue(MyPair<Rover*, int>(Erover, Erover->getSpeed()));
			cInCheckUp--;
			cPolarRovers++;
		}
		else
		{
			Pflag = false;
		}
		//Todo Check this statements 
		/*if (Eflag && !Erover || Erover->getAvailableAt() < Day)
			Eflag = false;
		else if (Eflag)
		{
			EmergencyRoversCheckUp.dequeue(Erover);
			EmergencyRovers.enqueue(MyPair<Rover*, int>(Erover, Erover->getSpeed()));
		}*/

	}
}

//Move rovers to checkUp after either a specific number of missions or if the mission failed
void MarsStation::MoveToCheckUp(Rover* R)
{
	R->setStatus(RoverStatus::InCheckUp);
	R->setAvailableAt(Day + R->getCheckUpDuration());
	//detemine the type of the rover and put it in the appropiate queue
	switch (R->getType())
	{
		case RoverType::Emergency :
			EmergencyRoversCheckUp.enqueue(R);
			break;
		case RoverType::Mountainous:
			MountinousRoverCheckUp.enqueue(R);
			break;
		case RoverType::Polar:
			PolarRoversCheckUp.enqueue(R);
			break;
		default:
			break;
	}
}


// Remove the link between the mission and the rover
// Check if this rover needs to have a checkup or not 
void MarsStation::DismissMissions(Mission* M)
{
	Rover* ReturnRover;
	ReturnRover = M->GetRover();
	//remove the link between the mission and the rover
	M->DismissRover();
	ReturnRover->increaseCompletedMissions();
	//check if this rover needs to have a check up or not
	if (MissionsBeforeCheckup <= ReturnRover->getCompletedMissions())
	{
		MoveToCheckUp(ReturnRover);
		cInCheckUp++;
	}
	//the rover do not need to have a check up
	else
	{
		ReturnRover->setStatus(RoverStatus::Available);
		//detemine the type of the rover and put it in the appropiate queue
		switch (ReturnRover->getType())
		{
			case RoverType::Emergency:
				EmergencyRovers.enqueue(MyPair<Rover*, int>(ReturnRover, ReturnRover->getSpeed()));
				cEmergencyRovers++;
				break;
			case RoverType::Mountainous:
				MountainousRovers.enqueue(MyPair<Rover*, int>(ReturnRover, ReturnRover->getSpeed()));
				cMountainousRovers++;
				break;
			case RoverType::Polar:
				PolarRovers.enqueue(MyPair<Rover*, int>(ReturnRover, ReturnRover->getSpeed()));
				cPolarRovers++;
				break;
			default:
				break;
		}
	}
}

//remove the mission from in-exectution queue then add it to the complete missions after doing its fulfill mission requirements
void MarsStation::MoveInExecutiontoComplete()
{
	Mission* M;
	while (true)
	{
		M = nullptr;
		InExecutionMissions.peekFront(M);
		//check if there is any in execution missions, if any,check if this day is the day on which a certain mission will complete its requirements
		if (M && M->GetCD() <= Day)
		{
			InExecutionMissions.dequeue(M);
			cInExecution--;
			M->SetMissionStatus(MissionStatus::Completed);
			CompletedMissions.push(M);
			cCompletedMissions++;
			//remove the link between the mission and the rover. Put the rover in the appropiate list
			DismissMissions(M);
		}
		else
		{
			break;
		}
	}
}

// Check if there is any mountainous mission that has been waiting more than the auto promotion duration
// If there is, remove it from the queue of the mountainous then enqeue it in the emergency waiting missions

void MarsStation::CheckUpAutoP()
{
	Mission* M;
	while (true)
	{
		M = nullptr;
		WaitingMountainousMissions.peekFront(M);
		//check if there is any mountainous mission , if any , compare the waiting days with the autoP
		if (M && M->GetWaitingDays() > AutoP)
		{
			WaitingMountainousMissions.dequeue(M);
			M->SetMissionType(MissionType::Emergency);
			WaitingEmergencyMissions.enqueue(MyPair<Mission*, int>(M, M->GetSignificance()));
			cAutop++;	//used to calculate the percentage of automatically-promoted missions (relative to the total number of mountainous missions)
		}
		else
		{
			break;
		}
	}
}

// Getters For UI 

void MarsStation::Simulate()
{
	InOut->InitialDisplayMessage();

	while (
		WaitingEmergencyMissionCount ||
		WaitingMountainousMissionCount ||
		WaitingPolarMissionCount ||
		!EventList.isEmpty() ||
		!InExecutionMissions.isEmpty()
		)
	{
		ExecuteEvent();
		MoveCheckUpToAvail();
		MissionFailure();
		MoveInExecutiontoComplete();
		CheckUpAutoP();
		AssignMissions();
		InOut->PrintCurrentDay(this);
		IncreaseDay();
		while (!CompletedMissions.isEmpty())
		{
			Mission* tempMission;
			CompletedMissions.pop(tempMission);
			delete tempMission;
		}
	}

	InOut->FinalDisplayMessage(this);
}

void MarsStation::MissionFailure()
{
	Queue<Mission*> tempQueue; //  no need or priorety queue
	Mission* tempMission;
	Rover* tempRover;
	double Percentage;
	//if rand less than 3% then the mission fails
	while (!InExecutionMissions.isEmpty())
	{
		InExecutionMissions.dequeue(tempMission);
		Percentage = (double(rand()) / RAND_MAX) * 100;
		if (Percentage <= 3)
		{
			//Remove the Rover then move it to checkup
			tempRover = tempMission->GetRover();
			MoveToCheckUp(tempRover);
			//Return the mission to waiting list to be assigned later
			tempMission->SetMissionStatus(MissionStatus::Waiting);
			tempMission->AssignRover(nullptr);
			AddMission(tempMission);
			cInExecution--;
			// CAUTION:this should be in UI NOT HERE
			std::cout << "\n######################################\n";
			std::cout << "#Mission Failed We'l Get'em Next Time#\n";
			std::cout << "######################################\n";
		}
		else
		{
			tempQueue.enqueue(tempMission);
		}

	}
	while (!tempQueue.isEmpty())
	{
		tempQueue.dequeue(tempMission);
		InExecutionMissions.enqueue(MyPair<Mission*, int>(tempMission, -1 * tempMission->GetCD()));
	}

}

Queue<Mission*> MarsStation::GetWaitingMissions(MissionType mType)
{
	switch (mType)
	{
	case MissionType::Emergency:
		return GetPriorityQueueAsQueue(WaitingEmergencyMissions);
	case MissionType::Mountainous:
		return WaitingMountainousMissions;
	case MissionType::Polar:
		return WaitingPolarMissions;
	default:
		throw("Unknow Mission Type");
	}
}

PriorityQueue<Mission*> MarsStation::GetInExecutionMissions()
{
	return InExecutionMissions;
}

PriorityQueue<Rover*> MarsStation::GetAvailableRovers(RoverType rType)
{
	switch (rType)
	{
	case RoverType::Emergency:
		return EmergencyRovers;
	case RoverType::Mountainous:
		return MountainousRovers;
	case RoverType::Polar:
		return PolarRovers;
	default:
		throw("Unkonwn Rover Type");
	}
}

Queue<Rover*> MarsStation::GetInCheckupRovers(RoverType rType)
{
	switch (rType)
	{
	case RoverType::Emergency:
		return EmergencyRoversCheckUp;
	case RoverType::Mountainous:
		return MountinousRoverCheckUp;
	case RoverType::Polar:
		return PolarRoversCheckUp;
	default:
		throw("Unkonwn Rover Type");
	}
}

Stack<Mission*> MarsStation::GetCompletedMissions()
{
	return CompletedMissions;
}

int MarsStation::GetWaitingMissionsCount()
{
	return cEmergencyMissions + cMountainousMissions + cPolarMissions;
}

int MarsStation::GetInExecitionMissionsCount()
{
	return cInExecution;
}

int MarsStation::GetAvailableRoversCount()
{
	return cEmergencyRovers + cMountainousRovers + cPolarRovers;
}

int MarsStation::GetInCheckupRoversCount()
{
	return cInCheckUp;
}

int MarsStation::GetCompletedMissionsCount()
{
	return cCompletedMissions;
}


// Destructor
MarsStation::~MarsStation()
{
	Rover* R;
	Event* E;
	Mission* M;
	while (EmergencyRovers.dequeue(R))
	{
		delete R;
	}
	while (MountainousRovers.dequeue(R))
	{
		delete R;
	}
	while (PolarRovers.dequeue(R))
	{
		delete R;
	}
	while (EmergencyRoversCheckUp.dequeue(R))
	{
		delete R;
	}
	while (MountinousRoverCheckUp.dequeue(R))
	{
		delete R;
	}
	while (PolarRoversCheckUp.dequeue(R))
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
	while (InExecutionMissions.dequeue(M))
	{
		delete M;
	}
}
