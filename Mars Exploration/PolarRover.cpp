#include"PolarRover.h"
int health = 0;
PolarRover::PolarRover(int s) : Rover(s)
{
	Health = health;
}

PolarRover::~PolarRover()
{
}
void PolarRover::Set_Health(int h)
{
	health = h;
}
RoverType PolarRover::getType() const
{
	return RoverType::Polar;
}

bool PolarRover::Fixed() const
{
	return (Health >= health);
}

void PolarRover::ModifyHealth(int h)
{
	Health += h;
	Health = (Health > health) ? health : Health;
	Health = (Health < 0) ? 0 : Health;
}

void PolarRover::RestoreHealth()
{
	Health = health;
}
