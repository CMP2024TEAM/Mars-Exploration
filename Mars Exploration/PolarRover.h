#include"rover.h"
class PolarRover : public Rover
{
public:
	PolarRover(int s);
	~PolarRover();
	static void Set_Health(int);
	int virtual GetMaxHealth() const;

private:
	virtual RoverType getType() const;
	virtual bool Fixed()const;
	virtual void ModifyHealth(int);
	virtual void RestoreHealth();
	static int health;
};

