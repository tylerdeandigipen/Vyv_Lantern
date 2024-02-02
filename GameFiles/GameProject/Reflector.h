#pragma once
#include "Component.h"
#include "Entity.h"

class Reflector : public Component
{
public:
	Reflector();
	Reflector(Reflector& reflector);
	~Reflector();

	// used to invoke the copy constructor
	Component* Clone(void) const = 0;

	void Read(json jsonData);
	//component-specific render code.
	void Update(float dt);

	void Render() const;
	void Read(json jsonData);
	static void ReflectorCollisionHandler(Entity & obj1, Entity & obj2);


private:

	bool isHit;
	//load position leftpoint rightpoint
	gfxVector2 position1;
	gfxVector2 position2;


};

