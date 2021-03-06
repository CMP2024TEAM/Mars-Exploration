#include "MarsStation.h"

// Constructor
MarsStation::MarsStation(UI* tInOut) :cInExecution(0), cEmergencyMissions(0), cMountainousMissions(0), cPolarMissions(0), cEmergencyRovers(0), cPolarRovers(0), cMountainousRovers(0), cExcecuteTime(0), cWaitTime(0), cAutop(0), Day(1)
{
	/* initialize random seed: */
	srand(time(NULL));
	//initail Values
	//srand(10); for const seed debuging
	cInCheckUp = 0;
	cInMaintenance = 0;
	cCompletedMissions = 0;
	TotalEmergencyMissions = 0;
	TotalMountainousMissions = 0;
	TotalPolarMissions = 0;
	TotalExcuationTime = 0;
	TotalWaitingTime = 0;
	WaitingEmergencyMissionCount = 0;
	WaitingMountainousMissionCount = 0;
	WaitingPolarMissionCount = 0;
	// To Remove The Warning Messages Only
	AutoP = 0;
	MissionsBeforeCheckup = 0;

	//this should be allocated outside and then return its pointer
	InOut = tInOut;
	InOut->ReadAll(this);

}

void MarsStation::CreateRover(RoverType type, int speed)
{
	Rover* R;
	switch (type)
	{
	case RoverType::Emergency:
		R = new EmergencyRover(speed);
		EmergencyRovers.enqueue(MyPair<Rover*, int>(R, speed));
		cEmergencyRovers++;
		break;
	case RoverType::Mountainous:
		R = new MountainousRover(speed);
		MountainousRovers.enqueue(MyPair<Rover*, int>(R, speed));
		cMountainousRovers++;
		break;
	case RoverType::Polar:
		R = new PolarRover(speed);
		PolarRovers.enqueue(MyPair<Rover*, int>(R, speed));
		cPolarRovers++;
		break;
	default:
		throw("Unkonwn Rover Type");
	}

}

// SetCheckupDuration for each rovar tyoe
void MarsStation::SetCheckupDuration(RoverType type, int Duration)
{
	switch (type)
	{
	case RoverType::Emergency:
		Rover::CheckupDuration[0] = Duration;
		break;
	case RoverType::Mountainous:
		Rover::CheckupDuration[1] = Duration;
		break;
	case RoverType::Polar:
		Rover::CheckupDuration[2] = Duration;
		break;
	default:
		throw("Unkonwn Rover Type");
		break;
	}
}

void MarsStation::SetRoverMaxHealth(RoverType(rType), int MaximumRoverHealth)
{
	switch (rType)
	{
	case RoverType::Emergency:
		EmergencyRover::Set_Health(MaximumRoverHealth);
		break;
	case RoverType::Mountainous:
		MountainousRover::Set_Health(MaximumRoverHealth);
		break;
	case RoverType::Polar:
		PolarRover::Set_Health(MaximumRoverHealth);
		break;
	default:
		throw("Type Not Found...");
		break;
	}
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
// AssignMission:Assigne each waiting mission to its right Rover
// Request Rover from Maintenace if Needed
void MarsStation::AssignMissions()
{
	Mission* M_ptr; //Mission pointer
	Rover* R_ptr;	//Rover Pointer
	// Assigning Emergency Missions
	while (!WaitingEmergencyMissions.isEmpty())
	{
		//Check for Emergncy Rovers First
		if (!EmergencyRovers.isEmpty())
		{
			//Remove Rover/Mission From Queue
			WaitingEmergencyMissions.dequeue(M_ptr);
			EmergencyRovers.dequeue(R_ptr);
			//Assign Mission/Rover
			M_ptr->AssignRover(R_ptr);
			M_ptr->SetMissionStatus(MissionStatus::InExecution);
			M_ptr->SetWaitingDays(Day - M_ptr->GetFormulationDay());
			M_ptr->SetED();
			InExecutionMissions.enqueue(MyPair<Mission*, int>(M_ptr, -1 * M_ptr->GetCD()));
			//Modify Counts
			cEmergencyRovers--;
			cEmergencyMissions--;
			cInExecution++;
			WaitingEmergencyMissionCount--;
		}//Check for Mountinous Rovers Second
		else if (!MountainousRovers.isEmpty())
		{
			//Remove Rover/Mission From Queue
			WaitingEmergencyMissions.dequeue(M_ptr);
			MountainousRovers.dequeue(R_ptr);
			//Assign Mission/Rover
			M_ptr->AssignRover(R_ptr);
			M_ptr->SetMissionStatus(MissionStatus::InExecution);
			M_ptr->SetWaitingDays(Day - M_ptr->GetFormulationDay());
			M_ptr->SetED();
			InExecutionMissions.enqueue(MyPair<Mission*, int>(M_ptr, -1 * M_ptr->GetCD()));
			//Modify Counts
			cMountainousRovers--;
			cEmergencyMissions--;
			cInExecution++;
			WaitingEmergencyMissionCount--;
		}//Check for Polar Rovers
		else if (!PolarRovers.isEmpty())
		{
			//Remove Rover/Mission From Queue
			WaitingEmergencyMissions.dequeue(M_ptr);
			PolarRovers.dequeue(R_ptr);
			M_ptr->AssignRover(R_ptr);
			//Assign Mission/Rover
			M_ptr->SetMissionStatus(MissionStatus::InExecution);
			M_ptr->SetWaitingDays(Day - M_ptr->GetFormulationDay());
			M_ptr->SetED();
			InExecutionMissions.enqueue(MyPair<Mission*, int>(M_ptr, -1 * M_ptr->GetCD()));
			//Modify Counts
			cPolarRovers--;
			cEmergencyMissions--;
			cInExecution++;
			WaitingEmergencyMissionCount--;
		}//Not Found Any Rover Request One!
		else
		{
			if (!RequestRover(RoverType::Emergency))
				break;
		}
	}

	//Assigning Polar Missions

	while (!WaitingPolarMissions.isEmpty())
	{
		if (!PolarRovers.isEmpty())
		{
			//Remove Rover/Mission From Queue
			WaitingPolarMissions.dequeue(M_ptr);
			PolarRovers.dequeue(R_ptr);
			//Assign Mission/Rover
			M_ptr->AssignRover(R_ptr);
			M_ptr->SetMissionStatus(MissionStatus::InExecution);
			M_ptr->SetWaitingDays(Day - M_ptr->GetFormulationDay());
			M_ptr->SetED();
			InExecutionMissions.enqueue(MyPair<Mission*, int>(M_ptr, -1 * M_ptr->GetCD()));
			//Modify Counts
			cPolarRovers--;
			cPolarMissions--;
			cInExecution++;
			WaitingPolarMissionCount--;
		}//Not Found Any Rover Request One!
		else
		{
			if (!RequestRover(RoverType::Polar))
				break;
		}
	}

	//Assigning Mountinous Missions

	while (!WaitingMountainousMissions.isEmpty())
	{
		//Check Mountinous Rovers First
		if (!MountainousRovers.isEmpty())
		{
			//Remove Rover/Mission From Queue
			WaitingMountainousMissions.dequeue(M_ptr);
			MountainousRovers.dequeue(R_ptr);
			//Assign Mission/Rover
			M_ptr->AssignRover(R_ptr);
			M_ptr->SetMissionStatus(MissionStatus::InExecution);
			M_ptr->SetWaitingDays(Day - M_ptr->GetFormulationDay());
			M_ptr->SetED();
			InExecutionMissions.enqueue(MyPair<Mission*, int>(M_ptr, -1 * M_ptr->GetCD()));
			//Modify Counts
			cMountainousRovers--;
			cMountainousMissions--;
			cInExecution++;
			WaitingMountainousMissionCount--;

		}//Check Emergency Rovers Second
		else if (!EmergencyRovers.isEmpty())
		{
			//Remove Rover/Mission From Queue
			WaitingMountainousMissions.dequeue(M_ptr);
			EmergencyRovers.dequeue(R_ptr);
			//Assign Mission/Rover
			M_ptr->AssignRover(R_ptr);
			M_ptr->SetMissionStatus(MissionStatus::InExecution);
			M_ptr->SetWaitingDays(Day - M_ptr->GetFormulationDay());
			M_ptr->SetED();
			InExecutionMissions.enqueue(MyPair<Mission*, int>(M_ptr, -1 * M_ptr->GetCD()));
			//Modify Counts
			cEmergencyRovers--;
			cMountainousMissions--;
			cInExecution++;
			WaitingMountainousMissionCount--;
		}//Not Found Any Rover Request One!
		else
		{
			if (!RequestRover(RoverType::Mountainous))
				break;
		}
	}
}
//Free Rover if Requested > Five Times return true if Freed false otherwise
bool MarsStation::RequestRover(RoverType r_type)
{
	Rover* R; //Rover Ptr
	switch (r_type)
	{
	case RoverType::Emergency:
		if (!EmergencyRoversMaintenance.peekFront(R))
			return RequestRover(RoverType::Mountainous);
		else
		{
			R->IncrementRequsted();
			if (R->GetRequsted() >= 5)
			{
				//Free Rover
				EmergencyRoversMaintenance.dequeue(R);
				R->RestoreHealth();
				R->ResetRequsted();
				R->DecrementSpeed();
				R->setAvailableAt(Day);
				R->setStatus(RoverStatus::Available);
				//Modify Counts
				cInMaintenance--;
				cEmergencyRovers++;
				EmergencyRovers.enqueue(MyPair<Rover*, int>(R, R->getSpeed()));
				return true;
			}
			return false;
		}
		break;
	case RoverType::Mountainous:
		if (!MountinousRoverMaintenance.peekFront(R))
		{
			if (!EmergencyRoversMaintenance.isEmpty())
				return RequestRover(RoverType::Emergency);
		}
		else
		{
			R->IncrementRequsted();
			if (R->GetRequsted() >= 5)
			{
				//free Rover
				MountinousRoverMaintenance.dequeue(R);
				R->RestoreHealth();
				R->ResetRequsted();
				R->DecrementSpeed();
				R->setAvailableAt(Day);
				R->setStatus(RoverStatus::Available);
				//Modify Counts
				cInMaintenance--;
				cMountainousRovers++;
				MountainousRovers.enqueue(MyPair<Rover*, int>(R, R->getSpeed()));
				return true;
			}
			return false;
		}
		break;
	case RoverType::Polar:
		if (!PolarRoverMaintenance.peekFront(R))
			return false;
		else
		{
			R->IncrementRequsted();
			if (R->GetRequsted() >= 5)
			{
				//free Rover
				PolarRoverMaintenance.dequeue(R);
				R->RestoreHealth();
				R->ResetRequsted();
				R->DecrementSpeed();
				R->setAvailableAt(Day);
				R->setStatus(RoverStatus::Available);
				//Modify Counts
				cInMaintenance--;
				cPolarRovers++;
				PolarRovers.enqueue(MyPair<Rover*, int>(R, R->getSpeed()));
				return true;
			}
			return false;
		}
		break;
	default:
		break;
	}
	return false;
}

// Add Mission To its corresponding list
void MarsStation::AddMission(Mission* mission)
{
	// Check Type Of Mission Then Add To Corrersponding List
	switch (mission->GetMissionType())
	{
	case MissionType::Emergency:
		EmergencyMission* EMission;
		EMission = dynamic_cast<EmergencyMission*>(mission);
		WaitingEmergencyMissions.enqueue(MyPair<Mission*, int>(mission, EMission->GetPriority()));
		WaitingEmergencyMissionCount++;
		cEmergencyMissions++;
		TotalEmergencyMissions++;
		break;
	case MissionType::Polar:
		WaitingPolarMissions.enqueue(mission);
		WaitingPolarMissionCount++;
		TotalPolarMissions++;
		cPolarMissions++;
		break;
	case MissionType::Mountainous:
		WaitingMountainousMissions.enqueue(mission);
		WaitingMountainousMissionCount++;
		TotalMountainousMissions++;
		cMountainousMissions++;
		break;
	default:
		break;
	}
}

// Get Waiting Mountainous Mission With Certain ID (Used In Events Execution)
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
	TotalMountainousMissions--;
	return true;
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
	Rover* R;
	while (!EmergencyRoversCheckUp.isEmpty())
	{
		EmergencyRoversCheckUp.peekFront(R);
		if (R->getAvailableAt() <= Day)
		{
			EmergencyRoversCheckUp.dequeue(R);
			if (R->GetHealth() <= 0)
			{
				MoveToMainetenace(R);
			}
			else
			{
				EmergencyRovers.enqueue(MyPair<Rover*, int>(R, R->getSpeed()));
				cEmergencyRovers++;
			}
			cInCheckUp--;
		}
		else
			break;
	}
	while (!MountinousRoverCheckUp.isEmpty())
	{
		MountinousRoverCheckUp.peekFront(R);
		if (R->getAvailableAt() <= Day)
		{
			MountinousRoverCheckUp.dequeue(R);
			if (R->GetHealth() <= 0)
			{
				MoveToMainetenace(R);
			}
			else
			{
				MountainousRovers.enqueue(MyPair<Rover*, int>(R, R->getSpeed()));
				cMountainousRovers++;
			}
			cInCheckUp--;
		}
		else
			break;
	}
	while (!PolarRoversCheckUp.isEmpty())
	{
		PolarRoversCheckUp.peekFront(R);
		if (R->getAvailableAt() <= Day)
		{
			PolarRoversCheckUp.dequeue(R);
			if (R->GetHealth() <= 0)
			{
				MoveToMainetenace(R);
			}
			else
			{
				PolarRovers.enqueue(MyPair<Rover*, int>(R, R->getSpeed()));
				cPolarRovers++;
			}
			cInCheckUp--;
		}
		else
			break;
	}
}

void MarsStation::MoveMaintenaceToAvail()
{
	Rover* R; //Rover ptr
	//Check if Rover finished Recovring Health(Maintenance)
	//Check Emerngecy
	while (!EmergencyRoversMaintenance.isEmpty())
	{
		EmergencyRoversMaintenance.peekFront(R);
		if (R->getAvailableAt() <= Day)
		{
			EmergencyRoversMaintenance.dequeue(R);
			R->ResetRequsted();
			R->RestoreHealth();
			R->setStatus(RoverStatus::Available);
			EmergencyRovers.enqueue(MyPair<Rover*, int>(R, R->getSpeed()));
			//Modify Counts
			cEmergencyRovers++;
			cInMaintenance--;
		}
		else
			break;
	}//Check Polar
	while (!PolarRoverMaintenance.isEmpty())
	{
		PolarRoverMaintenance.peekFront(R);
		if (R->getAvailableAt() <= Day)
		{

			PolarRoverMaintenance.dequeue(R);
			R->ResetRequsted();
			R->RestoreHealth();
			R->setStatus(RoverStatus::Available);
			PolarRovers.enqueue(MyPair<Rover*, int>(R, R->getSpeed()));
			//Modify Counts
			cInMaintenance--;
			cPolarRovers++;
		}
		else
			break;
	}//Check Mountinous
	while (!MountinousRoverMaintenance.isEmpty())
	{
		MountinousRoverMaintenance.peekFront(R);
		if (R->getAvailableAt() <= Day)
		{

			MountinousRoverMaintenance.dequeue(R);
			R->ResetRequsted();
			R->RestoreHealth();
			R->setStatus(RoverStatus::Available);
			MountainousRovers.enqueue(MyPair<Rover*, int>(R, R->getSpeed()));
			//Modify Counts
			cInMaintenance--;
			cMountainousRovers++;
		}
		else
			break;
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
	case RoverType::Emergency:
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
	cInCheckUp++;
	R->ResetCompletedMissions();
}
//IF Health is 0 Start Fixing the Rover
void MarsStation::MoveToMainetenace(Rover* R)
{
	R->setStatus(RoverStatus::InMaintenance);
	R->setAvailableAt(Day + R->GetMaxHealth() / 1000); //Maintenance Time + Current Day
	//detemine the type of the rover and put it in the appropiate queue
	switch (R->getType())
	{
	case RoverType::Emergency:
		EmergencyRoversMaintenance.enqueue(R);
		break;
	case RoverType::Mountainous:
		MountinousRoverMaintenance.enqueue(R);
		break;
	case RoverType::Polar:
		PolarRoverMaintenance.enqueue(R);
		break;
	default:
		break;
	}
	//Modify Counts
	cInMaintenance++;
}

// Remove the link between the mission and the rover then check if this rover needs to have a checkup or not 
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
	}
	else if (ReturnRover->GetHealth() <= 0)
	{
		MoveToMainetenace(ReturnRover);
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
			TotalWaitingTime += M->GetWaitingDays();
			TotalExcuationTime += M->GetED();
			//Calculating the damge from the mission
			M->GetRover()->ModifyHealth(-2 * M->GetTargetLocation());
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
	EmergencyMission* E;
	while (true)
	{
		M = nullptr;
		WaitingMountainousMissions.peekFront(M);
		//check if there is any mountainous mission , if any , compare the waiting days with the autoP
		if (M && (Day-M->GetFormulationDay()) >= AutoP)
		{
			WaitingMountainousMissions.dequeue(M);
			E = new EmergencyMission(M);
			delete M;
			AddMission(E);
			cAutop++;	//used to calculate the percentage of automatically-promoted missions (relative to the total number of mountainous missions)
			WaitingMountainousMissionCount--;
			cMountainousMissions--;
			TotalMountainousMissions--;
		}
		else
		{
			break;
		}
	}
}
//The Main Core of the Class which will be called in main
void MarsStation::Simulate()
{
	//Print
	InOut->InitialDisplayMessage();

	//Simulate Untill All Lists Are Empty
	while (
		!WaitingEmergencyMissions.isEmpty() ||
		!WaitingMountainousMissions.isEmpty() ||
		!WaitingPolarMissions.isEmpty() ||
		!EventList.isEmpty() ||
		!InExecutionMissions.isEmpty() ||
		!MountinousRoverCheckUp.isEmpty() ||
		!EmergencyRoversCheckUp.isEmpty() ||
		!PolarRoversCheckUp.isEmpty() ||
		!EmergencyRoversMaintenance.isEmpty() ||
		!MountinousRoverMaintenance.isEmpty() ||
		!PolarRoverMaintenance.isEmpty()
		)
	{
		//Main Functions
		ExecuteEvent();
		MoveCheckUpToAvail();
		MoveMaintenaceToAvail();
		MissionFailure();
		MoveInExecutiontoComplete();
		CheckUpAutoP();
		AssignMissions();
		//CurrentDay Info Print
		InOut->PrintCurrentDay(this);
		Day++;
		//Free Missions Every Day for Memory
		DeleteCompletedMissions();
	}
	//Print
	InOut->FinalDisplayMessage(this);
}

void MarsStation::DeleteCompletedMissions()
{
	//deallocate Memory
	while (!CompletedMissions.isEmpty())
	{
		Mission* M_ptr;
		CompletedMissions.pop(M_ptr);
		delete M_ptr;
	}
}

void MarsStation::MissionFailure()
{
	Queue<Mission*> M_Queue; //  no need for priorety queue
	Mission* M_ptr;		//Mission ptr
	Rover* R_ptr;		//Rover ptr
	double Percentage;	//100%
	//if rand less than 1% then the mission fails
	while (!InExecutionMissions.isEmpty())
	{
		InExecutionMissions.dequeue(M_ptr);
		Percentage = (double(rand()) / RAND_MAX) * 100;
		if (Percentage <= 1)
		{
			//Remove the Rover then move it to checkup or maintenace
			R_ptr = M_ptr->GetRover();
			R_ptr->ModifyHealth(-1000);
			MoveToCheckUp(R_ptr);
			//Return the mission to waiting list to be assigned later
			M_ptr->SetMissionStatus(MissionStatus::Waiting);
			M_ptr->AssignRover(nullptr);
			M_ptr->SetFormulationDay(Day);
			AddMission(M_ptr);
			// Update Counts and STATs
			cInExecution--;
			switch (M_ptr->GetMissionType())
			{
			case MissionType::Emergency:
				TotalEmergencyMissions--;
				break;
			case MissionType::Mountainous:
				TotalMountainousMissions--;
				break;
			case MissionType::Polar:
				TotalPolarMissions--;
				break;
			default:
				break;
			}
			//Print
			InOut->LogMissionFailure(M_ptr);
		}
		else
		{
			//Retrun back if not faild
			M_Queue.enqueue(M_ptr);
		}

	}
	while (!M_Queue.isEmpty())
	{
		//enqueue our mission after we check wich one failed we only enqueue those which didn't fail
		M_Queue.dequeue(M_ptr);
		InExecutionMissions.enqueue(MyPair<Mission*, int>(M_ptr, -1 * M_ptr->GetCD()));
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

int MarsStation::GetWaitingMissionsCount()const
{
	return cEmergencyMissions + cMountainousMissions + cPolarMissions;
}

int MarsStation::GetInExecitionMissionsCount()const
{
	return cInExecution;
}

int MarsStation::GetAvailableRoversCount()const
{
	return cEmergencyRovers + cMountainousRovers + cPolarRovers;
}

int MarsStation::GetInCheckupRoversCount() const
{
	return cInCheckUp;
}

int MarsStation::GetCompletedMissionsCount() const
{
	return cCompletedMissions;
}

int MarsStation::GetMountainouRoverCount() const
{
	return cMountainousRovers;
}

int MarsStation::GetPolarRoverCount() const
{
	return cPolarRovers;
}

int MarsStation::GetEmergencyRoverCount() const
{
	return cEmergencyRovers;
}

float MarsStation::GetAutoPPercent()const
{
	return (float)cAutop * 100 / cCompletedMissions;
}

int MarsStation::GetMountainouMissionCount() const
{
	return TotalMountainousMissions;
}

int MarsStation::GetEmergencyMissionCount() const
{
	return TotalEmergencyMissions;
}

int MarsStation::GetPolarMissionCount()const
{
	return TotalPolarMissions;
}

float MarsStation::GetAvgWait() const
{
	return (float)TotalWaitingTime / cCompletedMissions;
}

float MarsStation::GetAvgExec() const
{
	return (float)TotalExcuationTime / cCompletedMissions;
}

int MarsStation::GetInMaintenanceRoversCount() const
{
	return cInMaintenance;
}

Queue<Rover*> MarsStation::GetInMaintenanceRovers(RoverType type) const
{
	switch (type)
	{
	case RoverType::Emergency:
		return EmergencyRoversMaintenance;
	case RoverType::Mountainous:
		return MountinousRoverMaintenance;
	case RoverType::Polar:
		return PolarRoverMaintenance;
	default:
		throw("Unkonwn Rover Type");
	}
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
	while (EmergencyRoversMaintenance.dequeue(R))
	{
		delete R;
	}
	while (MountinousRoverMaintenance.dequeue(R))
	{
		delete R;
	}
	while (PolarRoverMaintenance.dequeue(R))
	{
		delete R;
	}
}
