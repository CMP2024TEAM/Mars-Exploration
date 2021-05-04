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

class UI;
class MarsStation
{
private:
	UI* InOut;
	Queue<Event*> EventList;
	Queue<Rover*> EmergencyRover;
	Queue<Rover*> MountainousRover;
	Queue<Rover*> PolarRover;
	int AutoP;
	int NumberOfMissionsTheRoverCompletesBeforeCheckup;
public:
	MarsStation()
	{
		//this should be allocated outside and then return its pointer
		//InOut = new UI;
		//InOut->Read();
	}
	//Initialize Rovar Queues
	void CreateRover(RoverType type, int speed)
	{
		Rover * R = new Rover(type, speed);
		if (type == RoverType::Emergency)
		{
			EmergencyRover.enqueue(R);
		}
		if (type == RoverType::Mountainous)
		{
			MountainousRover.enqueue(R);
		}
		if (type == RoverType::Polar)
		{
			PolarRover.enqueue(R);
		}
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
	void CreateFormulationEvent(MissionType mType,int ED,int ID,int TLOC,int MDUR,int SIG)
	{
		Event* E= new FormulationEvent(mType,ED,ID,TLOC,MDUR,SIG);
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
	~MarsStation()
	{
		Rover *R;
		Event *E;
		//delete InOut;
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
	}



};
