#include"rover.h"
class MountainousRover : public Rover
{
public:
	MountainousRover(int s);
	~MountainousRover();
	static void Set_Health(int);
	int virtual GetMaxHealth() const;

private:
	virtual RoverType getType() const;
	virtual bool Fixed()const;
	virtual void ModifyHealth(int);
	virtual void RestoreHealth();
	static int health;
};
