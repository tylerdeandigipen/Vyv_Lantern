#include "Emitter.h"
#include "Vector.h"

Emitter::Emitter() : Component(Component::cEmmitter), isDirty(false), emmitMaxDistance(0.0f)
{
	emmitDirection = new  gfxVector2();
	emmitPosition  = new gfxVector2();
}

Emitter::~Emitter()
{
	delete emmitDirection;
	delete emmitPosition;
}


void Emitter::Read(json jsonData)
{
	/*
	* forthose formating the json and looking here
	* 1st positon x,y
	* 2nd emitt direction vector
	* 3rd MaxDistance
	* 4th bool emitting T/F
	*/
	
	if (jsonData["emitter"].is_object())
	{
		json pos = jsonData["position"];
		emmitPosition->x = pos["x"];
		emmitPosition->y = pos["y"];
		json dir = jsonData["direction"];
		emmitDirection->x = dir["x"];
		emmitDirection->y = dir["y"];
	}
}

void Emitter::Update(float dt) 
{
	if (isDirty)
	{

	}
}