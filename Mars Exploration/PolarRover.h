#include"rover.h"
class PolarRover : public Rover
{
public:
	PolarRover(int s);
	~PolarRover();

private:
	virtual RoverType getType() const;
};

