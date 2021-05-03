#include"Mission.h"
#include"rover.h"
#include"UI.h"
#include"DataStructures/PriorityQueue.h"
#include"DataStructures/Queue.h"

class MarsStation
{
private:
	UI* InOut;
	Queue<Event> EventList;
	Queue<rover*> EmergencyRover;
	Queue<rover*> MountainousRover;
	Queue<rover*> PolarRover;
	int AutoP;
	int NumberOfMissionsTheRoverCompletesBeforeCheckup;
public:
	MarsStation()
	{
		InOut = new UI;
		InOut->Read();
	}
	//Initialize Rovar Queues
	void CreateRover(rovertype type, int speed)
	{
		rover * R = new rover(type, speed);
		if (type == Emergency)
		{
			EmergencyRover.enqueue(R);
		}
		if (type == Mountainous)
		{
			MountainousRover.enqueue(R);
		}
		if (type == Polar)
		{
			PolarRover.enqueue(R);
		}
	}
	//SetCheckupDuration for each rovar tyoe
	void SetCheckupDuration(rovertype type, int Duration)
	{
		if (type == Emergency)
			rover::CheckupDuration[0] = Duration;
		if (type == Mountainous)
			rover::CheckupDuration[1] = Duration;
		if (type == Polar)
			rover::CheckupDuration[2] = Duration;	
	}
	//set autopromosion
	void SetAutoP(int apDuration)
	{
		AutoP = (apDuration>0) ?apDuration :0 ;
	}
	void SetNumberOfMissionsTheRoverCompletesBeforeCheckup(int cNum)
	{
		NumberOfMissionsTheRoverCompletesBeforeCheckup = (cNum > 0) ? cNum : 0;
	}
	//create Events
	void CreateFormulationEvent(MissionType mType,int ED,int ID,int TLOC,int MDUR,int SIG)
	{
		Event *E= new FormulationEvent(mType,ED,ID,TLOC,MDUR,SIG);
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
		rover *R;
		Event *E;
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
	}



};
