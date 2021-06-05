#include"rover.h"
class PolarRover : public Rover
{
public:
	PolarRover(int s);
	~PolarRover();

private:
	virtual RoverType getType() const;
	virtual bool Fixed()const;
	virtual void ModifyHealth(int);
	virtual void RestoreHealth();
};

