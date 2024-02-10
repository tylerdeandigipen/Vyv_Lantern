#include "Emitter.h"
#include "Vector.h"
#include "Transform.h"
#include "Entity.h"
#include "LineCollider.h"
#include "Renderer.h"
#include "Logging.h"

Emitter::Emitter() : Component(Component::cEmitter)
{
	position = new gfxVector2();
	direction = new gfxVector2();
}

Emitter::Emitter(Emitter const& emitter2cpy) : Component(emitter2cpy)
, isDirty(emitter2cpy.isDirty), distance(emitter2cpy.distance), isEmitting(emitter2cpy.isEmitting)
{
	position = new gfxVector2(*emitter2cpy.position);
	direction = new gfxVector2(*emitter2cpy.direction);
}

Emitter::~Emitter()
{

	
}

Component* Emitter::Clone() const
{
	return new Emitter(*this);
}

void Emitter::Read(json jsonData)
{


}

void Emitter::Update(float dt) 
{

}

//second opinion maybe consider making a lazer a different component and emmit that component rather than this immiter 
void Emitter::Render() const
{


	/*if debug
	* 
	* 
	* 
	* 
	*/

}

/*
* learned code from 
*/
inline bool LineToLineCollision(Emitter * laser, Entity& line, int flag, bool interuptflag)
{

}

/*
* this funciton checks whetehr or not the emitted line will strike anything that casts a shadow.
* flag is a number gives right or left emitter status
* 1 = right
* 2 = left
*/
inline bool DoCalculations(Emitter* obj)
{
	
	
	
}

void Emitter::EmitterCollisionHandler(Entity& object1, Entity& object2)
{


	//this may be terrible currently ignores terrain
	//additionally does not have a way to turn off... will need to group source to fix issue can't think of how off the top of head.
	if (object1.Has(Emitter) && object2.Has(LineCollider))
	{
		





	}

	//the 2nd object is the one emitting
	if (object2.Has(Emitter) && object1.Has(LineCollider))
	{
		//copy the stuff from uptop
	}


	//potentially more stuff for line colliders maybe break this up into different behaviors.
	
}

