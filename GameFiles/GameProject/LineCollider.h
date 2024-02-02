#pragma once
#include "Component.h"
#include "Entity.h"
#include "Emitter.h"

class LineCollider : public Component
{
public:
	LineCollider();
	LineCollider(LineCollider& reflector);
	~LineCollider();

	// used to invoke the copy constructor
	Component* Clone(void) const = 0;

	void Read(json jsonData);
	//component-specific render code.
	void Update(float dt);
	
	void Render() const;
	void Read(json jsonData);

	static void LineColliderCollisionHandler(Entity & obj1, Entity & obj2);
	gfxVector2* GetPosition1() { return position1; };
	gfxVector2* GetPosition2() { return position2; };

private:
	
	Emitter* collidingWith;
	//load position leftpoint rightpoint
	gfxVector2* position1;
	gfxVector2* position2;

};

