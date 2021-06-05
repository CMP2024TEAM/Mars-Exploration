#include"PolarRover.h"
PolarRover::PolarRover(int s) : Rover(s)
{
	Health = 5000;
}

PolarRover::~PolarRover()
{
}
RoverType PolarRover::getType() const
{
	return RoverType::Polar;
}

bool PolarRover::Fixed() const
{
	return (Health >= 5000);
}

void PolarRover::ModifyHealth(int h)
{
	Health += h;
	Health = (Health > 5000) ? 5000 : Health;
	Health = (Health < 0) ? 0 : Health;
}

void PolarRover::RestoreHealth()
{
	Health = 5000;
}
