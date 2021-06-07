#include "MarsStation.h"
int main()
{
	UI InOut;
	MarsStation Station(&InOut);

	Station.Simulate();

	return 0;
}