#include"EmergencyRover.h"
int EmergencyRover::health = 0;
EmergencyRover::EmergencyRover(int s) : Rover(s)
{
	Health = health;
}

EmergencyRover::~EmergencyRover()
{
}
void EmergencyRover::Set_Health(int h)
{
	health = h;
}
int EmergencyRover::GetMaxHealth() const
{
	return health;
}
RoverType EmergencyRover::getType() const
{
	return RoverType::Emergency;
}

bool EmergencyRover::Fixed() const
{
	return (Health>=health);
}

void EmergencyRover::ModifyHealth(int h)
{
	Health += h;
	Health = (Health > health) ? health : Health;
	Health = (Health < 0) ? 0 : Health;
}

void EmergencyRover::RestoreHealth()
{
	Health = health;
}
