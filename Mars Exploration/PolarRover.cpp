#include"PolarRover.h"
PolarRover::PolarRover(int s) : Rover(s)
{
}

PolarRover::~PolarRover()
{
}
RoverType PolarRover::getType() const
{
	return RoverType::Polar;
}