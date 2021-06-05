#include"MountainousRover.h"

MountainousRover::MountainousRover(int s) : Rover(s)
{
	Health = 6000;
}

MountainousRover::~MountainousRover()
{
}
RoverType MountainousRover::getType() const
{
	return RoverType::Mountainous;
}

bool MountainousRover::Fixed() const
{
	return (Health >= 6000);
}

void MountainousRover::ModifyHealth(int h)
{
	Health += h;
	Health = (Health > 6000) ? 6000 : Health;
	Health = (Health < 0) ? 0 : Health;
}

void MountainousRover::RestoreHealth()
{
	Health = 6000;
}
