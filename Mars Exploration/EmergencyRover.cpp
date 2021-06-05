#include"EmergencyRover.h"
EmergencyRover::EmergencyRover(int s) : Rover(s)
{
	Health = 7000;
}

EmergencyRover::~EmergencyRover()
{
}
RoverType EmergencyRover::getType() const
{
	return RoverType::Emergency;
}

bool EmergencyRover::Fixed() const
{
	return (Health>=7000);
}

void EmergencyRover::ModifyHealth(int h)
{
	Health += h;
	Health = (Health > 7000) ? 7000 : Health;
	Health = (Health < 0) ? 0 : Health;
}

void EmergencyRover::RestoreHealth()
{
	Health = 7000;
}
