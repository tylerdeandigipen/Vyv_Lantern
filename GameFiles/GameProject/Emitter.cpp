#include "Emitter.h"
#include "Vector.h"
#include "Transform.h"
#include "Entity.h"

Emitter::Emitter() : Component(Component::cEmmitter)
{
	emitMaxDistance = 0.0f;
	isDirty = false;
	isEmitting = false;
	DrawDistance = 0.0f;
	emitDirection = new  gfxVector2();
	emitDirection2 = new gfxVector2();
	emitPosition  = new gfxVector2();
	isDualMirror = false;
	emitterTwoIsActive = false;
}

Emitter::Emitter(Emitter const& emitter2cpy) : Component(emitter2cpy)
, isDirty(emitter2cpy.isDirty), isEmitting(emitter2cpy.isEmitting), emitMaxDistance(emitter2cpy.emitMaxDistance),
DrawDistance(emitter2cpy.DrawDistance), isDualMirror(emitter2cpy.isDualMirror), emitterTwoIsActive(emitter2cpy.emitterTwoIsActive)
{
	emitDirection2 = new gfxVector2(*emitter2cpy.emitDirection2);
	emitDirection = new gfxVector2(*emitter2cpy.emitDirection);
	emitPosition = new gfxVector2(*emitter2cpy.emitPosition);
}

Emitter::~Emitter()
{
	delete emitDirection;
	delete emitDirection2;
	delete emitPosition;
}

Component* Emitter::Clone() const
{
	return new Emitter(*this);
}

void Emitter::Read(json jsonData)
{
	/*
	* forthose formating the json and looking here
	* 1st positon x,y generally the center of the object it's attached to it.
	* 2nd emitt direction vector
	* 3rd MaxDistance
	* 4th bool emitting T/F default to false only make true if you want this to be a constant source that you can push a mirror in front of.
	*/
	
	if (jsonData["position"].is_object())
	{
		json pos = jsonData["position"];
		emitPosition->x = pos["x"];
		emitPosition->y = pos["y"];
	}

	if (jsonData["direction"].is_object())
	{
		json dir = jsonData["direction"];
		emitDirection->x = dir["x"];
		emitDirection->y = dir["y"];
	}

	if (jsonData["direction2"].is_object())
	{
		json dir2 = jsonData["direction"];
		emitDirection2->x = dir2["x"];
		emitDirection2->y = dir2["y"];
	}

	emitMaxDistance = jsonData["maxDistance"];
	isEmitting = jsonData["isEmitting"];
	isDualMirror = jsonData["dualMirror"];
	emitterTwoIsActive = jsonData["emittingSecond"];
}

void Emitter::Update(float dt) 
{
	if (isDirty)
	{
		//update position and colisions
	}


}

//second opinion maybe consider making a lazer a different component and emmit that component rather than this immiter 
void Emitter::Render() const
{
	if (isEmitting)
	{
		//draw the laser and do draw laser calculations
	}

	if (emitterTwoIsActive)
	{
		//draw second laser
	}

}


void EmitterCollisionHandler(Entity& object1, Entity& object2)
{
	//if emitter to reflector
	if (/*entity1->GetRealName().compare("") == 0 && entity2->GetRealName().compare("Object") == 0*/ false)
	{
			
	}

	//reflector ot emitter
	if (/*entity1->GetRealName().compare("Player") == 0 && entity2->GetRealName().compare("Object") == 0 */ false)
	{
		object1->Has(Emitter)
	}
	
}