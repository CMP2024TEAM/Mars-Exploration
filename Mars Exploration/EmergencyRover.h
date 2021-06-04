#include"rover.h"
class EmergencyRover : public Rover
{
public:
	EmergencyRover(int s);
	~EmergencyRover();

private:
	virtual RoverType getType() const;
};

