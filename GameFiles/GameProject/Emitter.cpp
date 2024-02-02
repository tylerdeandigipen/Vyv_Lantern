#include "Emitter.h"
#include "Vector.h"
#include "Transform.h"
#include "Entity.h"
#include "LineCollider.h"

Emitter::Emitter() : Component(Component::cEmitter)
{
	emitMaxDistance = 0.0f;
	isDirty = false;
	isEmitting = false;
	DrawDistance = 0.0f;
	emitLTrigDir = new  gfxVector2();
	emitRTrigDir = new gfxVector2();
	emitPosition  = new gfxVector2();
	leftTrigger = false;
	rightTrigger = false;
	emitterTwoIsActive = false;
}

Emitter::Emitter(Emitter const& emitter2cpy) : Component(emitter2cpy)
, isDirty(emitter2cpy.isDirty), isEmitting(emitter2cpy.isEmitting), emitMaxDistance(emitter2cpy.emitMaxDistance),
DrawDistance(emitter2cpy.DrawDistance), emitterTwoIsActive(emitter2cpy.emitterTwoIsActive),
leftTrigger(emitter2cpy.leftTrigger), rightTrigger(emitter2cpy.rightTrigger)
{
	emitLTrigDir = new gfxVector2(*emitter2cpy.emitLTrigDir);
	emitRTrigDir = new gfxVector2(*emitter2cpy.emitRTrigDir);
	emitPosition = new gfxVector2(*emitter2cpy.emitPosition);
}

Emitter::~Emitter()
{
	delete emitRTrigDir;
	delete emitLTrigDir;
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

	if (jsonData["LeftDirectionTrigger"].is_object())
	{
		json dir = jsonData["directionLeft"];
		emitLTrigDir->x = dir["x"];
		emitLTrigDir->y = dir["y"];
	}

	if (jsonData["RightDirectionTrigger"].is_object())
	{
		json dir2 = jsonData["directionRight"];
		emitRTrigDir->x = dir2["x"];
		emitRTrigDir->y = dir2["y"];
	}

	emitMaxDistance = jsonData["maxDistance"];
	isEmitting = jsonData["isEmitting"];
	emitterTwoIsActive = jsonData["emittingSecond"];
}

void Emitter::Update(float dt) 
{
	//cleans up the hasCollided flag for the emitter
	//should turn off when you stop emitting.
	if (hasCollided)
	{
		hasCollided = isEmitting;
	}
}

//second opinion maybe consider making a lazer a different component and emmit that component rather than this immiter 
void Emitter::Render() const
{
	if (isEmitting)
	{
		//draw the laser and do draw laser calculations
		if (leftTrigger)
		{

		}
		if (rightTrigger)
		{

		}
	}


}


void Emitter::EmitterCollisionHandler(Entity& object1, Entity& object2)
{


	//this may be terrible currently ignores terrain
	//additionally does not have a way to turn off... will need to group source to fix issue can't think of how off the top of head.
	if (object1.Has(Emitter) && object2.Has(LineCollider))
	{
		//for making this mor effecient should store the emitter and collider locally for the calculations


		//if the object is emitting and hasn't collided yet. proceed
		if (object1.Has(Emitter)->IsEmitterEmitting() && !object1.Has(Emitter)->GetCollided())
		{
			//need to check the double triggers from left or right.

			if (object1.Has(Emitter)->rightTrigger)
			{
			}
			if (object1.Has(Emitter)->leftTrigger)
			{
			}

			//also need to check vs terrain if it hits needs to truncate the draw distance of the emitter line.

			if (false /* hits terrain*/)
			{
				object1.Has(Emitter)->SetDrawDistance(0.0f /* truncate to collision point of terrain */);
				object1.Has(Emitter)->SetCollided(true);
			}
			//we do not care if it has a line collider if it doesn't have an emitter, proceed to leave early.
			if (object2.Has(Emitter) != nullptr)
			{
				//do collision calculations
				if (true /*did collide*/)
				{
					//calculate which direction using dot product
					//lineColliders consist of 2 points that make a line. point one should be the left most point.
					gfxVector2 tempVector = *object2.Has(LineCollider)->GetPosition1() - *object2.Has(LineCollider)->GetPosition2();
					float leftRight = gfxVector2::DotProduct(*object1.Has(Emitter)->GetDirection(), tempVector);

					if (leftRight > 0.0f) // greater than less than 0 for right side collision
					{
						//collides
						object1.Has(Emitter)->SetCollided(true);
						object2.Has(Emitter)->SetEmitting(true);
						object2.Has(Emitter)->SetRightTrigger(true);
					}
					else /* must be side left */
					{
						object1.Has(Emitter)->SetCollided(true);
						object2.Has(Emitter)->SetEmitting(true);
						object2.Has(Emitter)->SetLeftTrigger(true);//placeholding unsure of left right
					}

				}

			}
		}

		return; // cut out quickly if the 1st is true but anything else does not then leave early.
	}

	//the 2nd object is the one emitting
	if (object2.Has(Emitter)->IsEmitterEmitting() && !object2.Has(Emitter)->GetCollided() && object1.Has(LineCollider))
	{
		//copy the stuff from uptop
	}


	//potentially more stuff for line colliders maybe break this up into different behaviors.
	
}