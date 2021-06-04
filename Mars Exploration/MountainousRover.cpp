#include"MountainousRover.h"

MountainousRover::MountainousRover(int s) : Rover(s)
{
}

MountainousRover::~MountainousRover()
{
}
RoverType MountainousRover::getType() const
{
	return RoverType::Mountainous;
}