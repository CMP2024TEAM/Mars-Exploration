#include"MountainousRover.h"
int MountainousRover::health = 0;
MountainousRover::MountainousRover(int s) : Rover(s)
{
	Health = health;
}

MountainousRover::~MountainousRover()
{
}
void MountainousRover::Set_Health(int h)
{
	health = h;
}
RoverType MountainousRover::getType() const
{
	return RoverType::Mountainous;
}

bool MountainousRover::Fixed() const
{
	return (Health >= health);
}

void MountainousRover::ModifyHealth(int h)
{
	Health += h;
	Health = (Health > health) ? health : Health;
	Health = (Health < 0) ? 0 : Health;
}

void MountainousRover::RestoreHealth()
{
	Health = health;
}
