#include"rover.h"
class EmergencyRover : public Rover
{
public:
	EmergencyRover(int s);
	~EmergencyRover();
	static void Set_Health(int);
	int virtual GetMaxHealth() const;
private:
	virtual RoverType getType() const;
	virtual bool Fixed()const;
	virtual void ModifyHealth(int);
	virtual void RestoreHealth();
	static int health;
};

