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
#include<stdlib.h>
#include<time.h>
#include"EmergencyRover.h"
#include"MountainousRover.h"
#include"PolarRover.h"
#include "EmergencyMission.h"
#include "MountainousMission.h"
#include "PolarMission.h"

class MarsStation
{
private:
	// User Interface
	UI* InOut;
	// Lists
	Queue<Event*> EventList;
	PriorityQueue<Mission*> WaitingEmergencyMissions;
	Queue<Mission*> WaitingPolarMissions;
	Queue<Mission*> WaitingMountainousMissions;
	PriorityQueue<Rover*> EmergencyRovers;
	PriorityQueue<Rover*> MountainousRovers;
	PriorityQueue<Rover*> PolarRovers;
	PriorityQueue<Mission*> InExecutionMissions;
	Queue<Rover*> EmergencyRoversCheckUp;
	Queue<Rover*> MountinousRoverCheckUp;
	Queue<Rover*> PolarRoversCheckUp;
	Queue<Rover*> EmergencyRoversMaintenance;
	Queue<Rover*> MountinousRoverMaintenance;
	Queue<Rover*> PolarRoverMaintenance;
	Stack<Mission*> CompletedMissions;
	int AutoP;
	int MissionsBeforeCheckup;
	// For while stop condition
	int cInExecution;
	// Counts
	int WaitingEmergencyMissionCount;
	int WaitingMountainousMissionCount;
	int WaitingPolarMissionCount;
	// For Stats
	int cAutop;
	int cInCheckUp;
	int cInMaintenance;
	int cEmergencyMissions;
	int cMountainousMissions;
	int cPolarMissions;
	int cExcecuteTime;
	int cWaitTime;
	int cEmergencyRovers;
	int cPolarRovers;
	int cMountainousRovers;
	int cCompletedMissions;
	int TotalEmergencyMissions;
	int TotalMountainousMissions;
	int TotalPolarMissions;
	int TotalExcuationTime;
	int TotalWaitingTime;
	int Day;
public:
	// Constructor
	MarsStation(UI*);
	
	// Initialize Rovar Queues
	void CreateRover(RoverType type, int speed);
	
	// SetCheckupDuration for each rovar tyoe
	void SetCheckupDuration(RoverType type, int Duration);
	
	//Initialize Rover Health
	void SetRoverMaxHealth(RoverType(rType), int MaximumRoverHealth);

	// Set autopromosion
	void SetAutoP(int apDuration);
	void SetMissionsBeforeCheckup(int cNum);
	
	// Create Events
	void CreateFormulationEvent(MissionType mType, int ED, int ID, int TLOC, int MDUR, int SIG);
	void CreateCancellationEvent(int ED, int ID);
	void CreatePromotionEvent(int ED, int ID);
	
	// Call this function at the start of every new day
	void AssignMissions();

	//Request a rover more than five times will free the rover from maintencae but will decrese its speed
	bool RequestRover(RoverType);

	// Add Mission To its corresponding list
	void AddMission(Mission* mission);
	
	// Get Waiting Mountainous Mission With Certain ID (USed In Events Execution)
	bool GetMountainouMission(Mission*& mission, int ID);

	// Get Current day
	int GetCurrentDay();

	// Execute events stored in Queue
	void ExecuteEvent();

	// Move rovers in check up to be available to assign a mission 
	void MoveCheckUpToAvail();

	//Move Rovers in Maintenace to be available to assign a mission 
	void MoveMaintenaceToAvail();

	//Move rovers to checkUp after either a specific number of missions or if the mission failed
	void MoveToCheckUp(Rover* R);

	//Moves Rover to maintenace if Health is 0
	void MoveToMainetenace(Rover* R);

	// Remove the link between the mission and the rover
	// Check if this rover needs to have a checkup or not 
	void DismissMissions(Mission* M);

	//remove the mission from in-exectution queue then add it to the complete missions after doing its fulfill mission requirements
	void MoveInExecutiontoComplete();

	// Check if there is any mountainous mission that has been waiting more than the auto promotion duration
	// If there is, remove it from the queue of the mountainous then enqeue it in the emergency waiting missions
	void CheckUpAutoP();

	//Simulate: This should iterate all neccessary function 
	void Simulate();

	void DeleteCompletedMissions();

	//Mission Failure: Random Faluire of a mission because of a rover malfunction
	void MissionFailure();

	// Getters For UI 
	Queue<Mission*> GetWaitingMissions(MissionType mType);
	PriorityQueue<Mission*> GetInExecutionMissions();
	PriorityQueue<Rover*> GetAvailableRovers(RoverType rType);
	Queue<Rover*> GetInCheckupRovers(RoverType rType);
	Stack<Mission*> GetCompletedMissions();
	// Create Queue Of elements Stored In Priority Queue
	template <class T>
	Queue<T> GetPriorityQueueAsQueue(PriorityQueue<T> missions);
	// Getters For UI 
	int GetWaitingMissionsCount() const;
	int GetInExecitionMissionsCount()const;
	int GetAvailableRoversCount() const;
	int GetInCheckupRoversCount()const;
	int GetCompletedMissionsCount()const;
	int GetMountainouRoverCount() const;
	int GetPolarRoverCount() const;
	int GetEmergencyRoverCount() const;
	float GetAutoPPercent() const;
	int GetMountainouMissionCount() const;
	int GetEmergencyMissionCount() const;
	int GetPolarMissionCount() const;
	float GetAvgWait() const;
	float GetAvgExec() const;
	int GetInMaintenanceRoversCount() const;
	Queue<Rover*> GetInMaintenanceRovers(RoverType type) const;
	// Destructor
	~MarsStation();
};

// Create Queue Of elements Stored In Priority Queue

template<class T>
inline Queue<T> MarsStation::GetPriorityQueueAsQueue(PriorityQueue<T> missions)
{
	PriorityQueue<T> temp;
	Queue<T> result;
	MyPair<T, int> m;

	while (missions.dequeue(m))
	{
		temp.enqueue(m);
		result.enqueue(m.first);
	}

	while (temp.dequeue(m))
	{
		missions.enqueue(m);
	}

	return result;
}
