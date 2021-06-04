#include"rover.h"
class MountainousRover : public Rover
{
public:
	MountainousRover(int s);
	~MountainousRover();

private:
	virtual RoverType getType() const;
};
