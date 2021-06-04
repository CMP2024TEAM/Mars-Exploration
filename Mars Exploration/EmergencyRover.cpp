#include"EmergencyRover.h"
EmergencyRover::EmergencyRover(int s) : Rover(s)
{
}

EmergencyRover::~EmergencyRover()
{
}
RoverType EmergencyRover::getType() const
{
	return RoverType::Emergency;
}