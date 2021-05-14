#include "MarsStation.h"
int main()
{
	int x;
	OutputType y;
	std::cin >> x;
	switch (x)
	{
	case 0:y = OutputType(0); break;
	case 1:y = OutputType(1); break;
	case 2:y = OutputType(2); break;
	default:y = OutputType(2);break;
	}

	UI* InOut = new UI(y);
	MarsStation *Station=new MarsStation(InOut);

	Station->Simulate();


	delete InOut;
	delete Station;
	return 0;
}