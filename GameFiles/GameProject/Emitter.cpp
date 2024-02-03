#include "Emitter.h"
#include "Vector.h"
#include "Transform.h"
#include "Entity.h"
#include "LineCollider.h"
#include "Renderer.h"

Emitter::Emitter() : Component(Component::cEmitter)
{
	emitMaxDistance = 0.0f;
	isDirty = false;
	isEmittingRight = false;
	isEmittingLeft = false;
	DrawDistance = 0.0f;
	emitLTrigDir = new  gfxVector2();
	emitRTrigDir = new gfxVector2();
	emitPositionRight  = new gfxVector2();
	emitPositionLeft = new gfxVector2();
	emitpositionEndL = new gfxVector2();
	emitpositionEndR = new gfxVector2();
	leftTrigger = false;
	rightTrigger = false;
	hasCollidedRight = false;
	hasCollidedLeft = false;
}

Emitter::Emitter(Emitter const& emitter2cpy) : Component(emitter2cpy)
, isDirty(emitter2cpy.isDirty), isEmittingRight(emitter2cpy.isEmittingRight), emitMaxDistance(emitter2cpy.emitMaxDistance),
DrawDistance(emitter2cpy.DrawDistance),
leftTrigger(emitter2cpy.leftTrigger), rightTrigger(emitter2cpy.rightTrigger), hasCollidedRight(emitter2cpy.hasCollidedRight),
hasCollidedLeft(emitter2cpy.hasCollidedLeft), isEmittingLeft(emitter2cpy.isEmittingLeft)
{
	emitLTrigDir = new gfxVector2(*emitter2cpy.emitLTrigDir);
	emitRTrigDir = new gfxVector2(*emitter2cpy.emitRTrigDir);
	emitPositionRight = new gfxVector2(*emitter2cpy.emitPositionRight);
	emitPositionLeft = new gfxVector2(*emitter2cpy.emitPositionLeft);
}

Emitter::~Emitter()
{
	delete emitRTrigDir;
	delete emitLTrigDir;
	delete emitPositionRight;
	delete emitPositionLeft;
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
	
	if (jsonData["positionRight"].is_object())
	{
		json pos = jsonData["positionRight"];
		emitPositionRight->x = pos["x"];
		emitPositionRight->y = pos["y"];
	}

	if (jsonData["positionLeft"].is_object())
	{
		json pos = jsonData["positionLeft"];
		emitPositionLeft->x = pos["x"];
		emitPositionLeft->y = pos["y"];
	}

	if (jsonData["LeftDirectionTrigger"].is_object())
	{
		json dir = jsonData["LeftDirectionTrigger"];
		emitLTrigDir->x = dir["x"];
		emitLTrigDir->y = dir["y"];
	}

	if (jsonData["RightDirectionTrigger"].is_object())
	{
		json dir2 = jsonData["RightDirectionTrigger"];
		emitRTrigDir->x = dir2["x"];
		emitRTrigDir->y = dir2["y"];
	}

	emitMaxDistance = jsonData["maxDistance"];

	if (jsonData["emitRight"].is_boolean())
	{
		isEmittingRight = jsonData["emitRight"];
	}
	if (jsonData["trigRight"].is_boolean())
	{
		rightTrigger = jsonData["trigRight"];
	}


}

void Emitter::Update(float dt) 
{
	//cleans up the hasCollided flag for the emitter
	//should turn off when you stop emitting.
	if (hasCollidedRight)
	{
		hasCollidedRight = isEmittingRight;
	}
}

//second opinion maybe consider making a lazer a different component and emmit that component rather than this immiter 
void Emitter::Render() const
{
	if (isEmittingRight)
	{

	}

	if (isEmittingLeft)
	{
	}

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
inline bool LineToLineCollision(Emitter * laser, Entity& line, int flag)
{
	LineCollider*  literalLine = line.Has(LineCollider);
	//first line
	float x1 = literalLine->GetPosition1()->x;
	float y1 = literalLine->GetPosition1()->y;
	float x2 = literalLine->GetPosition2()->x;
	float y2 = literalLine->GetPosition2()->y;

	//second line
	float x3 = 0.0f;
	float y3 = 0.0f;
	
	switch (flag)
	{
	case 1:
		x3 = laser->GetPositionRight()->x;
		y3 = laser->GetPositionRight()->y;
		break;
	case 2: 
		x3 = laser->GetPositionLeft()->x;
		y3 = laser->GetPositionLeft()->y;
		break;
	default:
		break;
	}


	float x4 = 0.0f;
	float y4 = 0.0f;

	bool withinx = false;
	bool withiny = false;
	bool didcollide = false;

	if (y1 > y3 && y2 > y3)
	{
		withiny = true;
	}
	if (x1 > x3 && x2 > x3)
	{
		withinx = true;
	}

	float x = 0;
	float y = 0;
	if (flag == 1)
	{
		x = laser->GetDirectionRight()->x;
		y = laser->GetDirectionRight()->y;

	}
	else if (flag == 2)
	{
		x = laser->GetDirectionLeft()->x;
		y = laser->GetDirectionRight()->y;
	}

	//write truncating distance code.
	if (withinx)
	{
		x4 = x3;
		y4 = y3 + (laser->GetMaxEmitDistance() * y);
	}


	if (withiny)
	{
		y4 = y3;
		x4 = x3 + (laser->GetMaxEmitDistance() * y);
	}

	//distance
	float uA = ((x4 - x3) * (y1 - y3) - (y4 - y3) * (x1 - x3)) / ((y4 - y3) * (x2 - x1) - (x4 - x3) * (y2 - y1));
	float uB = ((x2 - x1) * (y1 - y3) - (y2 - y1) * (x1 - x3)) / ((y4 - y3) * (x2 - x1) - (x4 - x3) * (y2 - y1));

	if (uA >= 0 && uA <= 1 && uB >= 0 && uB <= 1) {
		didcollide = true;
		float intersectionX = x1 + (uA * (x2 - x1));
		float intersectionY = y1 + (uA * (y2 - y1));
		
		if (line.Has(Emitter))
		{
			switch (flag)
			{

			case 1:
				// 1,0 east
				// 0,1 north
				// -1,0 west
				// 0,-1 south
				//get endpoint check against 
				if (laser->GetEmitPositionEndR()->x < intersectionX)
				{
					laser->SetEmitPositionEndR(intersectionX, intersectionY);
					line.Has(Emitter)->SetPositionRight(intersectionX, intersectionY);
					return true;
				}
				else
				{
					return false;
				}

				break;
			case 2:
				if (laser->GetEmitPositionEndL()->x < intersectionX)
				{
					laser->SetEmitPositionEndL(intersectionX, intersectionY);
					line.Has(Emitter)->SetPositionLeft(intersectionX, intersectionY);
				}
				else
				{
					return false;
				}

				break;
			default:

				break;
			}

		}
		
	}





	return didcollide;
}

/*
* this funciton checks whetehr or not the emitted line will strike anything that casts a shadow.
* flag is a number gives right or left emitter status
* 1 = right
* 2 = left
*/
inline bool DoCalculations(Emitter* obj,int flag)
{

	float x = 0;
	float y = 0;
	if (flag == 1)
	{
		x = obj->GetDirectionRight()->x;
		y = obj->GetDirectionRight()->y;

	}
	else if (flag == 2)
	{
		x = obj->GetDirectionLeft()->x;
		y = obj->GetDirectionRight()->y;
	}
	
	Renderer* renderer = Renderer::GetInstance();

	//banks on the fact that we are using North South East West.
	if (x > 0)
	{
		gfxVector2 comparison = renderer->CheckLineForObjects(x, y, x + obj->GetMaxEmitDistance(), y);
		if (!(comparison.x == x+obj->GetMaxEmitDistance()) && !(comparison.y == y))
		{
			//comback and revise this later
			switch (flag)
			{
			case 1:
				obj->SetEmitPositionEndR(comparison.x, comparison.y);
				obj->SetCollidedRight(true);
				break;
			case 2:
				obj->SetEmitPositionEndL(comparison.x, comparison.y);
				obj->SetCollidedLeft(true);
				break;
			default:
				break;
			}

			return true;
		}
	}
	else if (x < 0)
	{

		gfxVector2 comparison = renderer->CheckLineForObjects(x, y, x - obj->GetMaxEmitDistance(), y);
		if ((comparison.x == x + obj->GetMaxEmitDistance()) && (comparison.y == y))
		{
			//comback and revise this later
			switch (flag)
			{
			case 1:
				obj->SetEmitPositionEndR(comparison.x, comparison.y);
				obj->SetCollidedRight(true);
				break;
			case 2:
				obj->SetEmitPositionEndL(comparison.x, comparison.y);
				obj->SetCollidedLeft(true);
				break;
			default:
				break;
			}

			return true;
		}
	}

	if (y > 0)
	{
		gfxVector2 comparison = renderer->CheckLineForObjects(x, y, x, y + obj->GetMaxEmitDistance());
		if (!(comparison.x == x + obj->GetMaxEmitDistance()) && !(comparison.y == y))
		{
			//comback and revise this later
			switch (flag)
			{
			case 1:
				obj->SetEmitPositionEndR(comparison.x, comparison.y);
				obj->SetCollidedRight(true);
				break;
			case 2:
				obj->SetEmitPositionEndL(comparison.x, comparison.y);
				obj->SetCollidedLeft(true);
				break;
			default:
				break;
			}

			return true;
		}
	}
	else if (y < 0)
	{
		gfxVector2 comparison = renderer->CheckLineForObjects(x, y, x, y - obj->GetMaxEmitDistance());
		if (!(comparison.x == x + obj->GetMaxEmitDistance()) && !(comparison.y == y))
		{
			//comback and revise this later
			switch (flag)
			{
			case 1:
				obj->SetEmitPositionEndR(comparison.x, comparison.y);
				obj->SetCollidedRight(true);
				break;
			case 2:
				obj->SetEmitPositionEndL(comparison.x, comparison.y);
				obj->SetCollidedLeft(true);
				break;
			default:
				break;
			}

			return true;
		}
	}


	return false;
	
}

void Emitter::EmitterCollisionHandler(Entity& object1, Entity& object2)
{


	//this may be terrible currently ignores terrain
	//additionally does not have a way to turn off... will need to group source to fix issue can't think of how off the top of head.
	if (object1.Has(Emitter) && object2.Has(LineCollider))
	{
		//for making this mor effecient should store the emitter and collider locally for the calculations
		Emitter* Obj1 = object1.Has(Emitter);

		int flag = 0; // 1 = right 2 = left

		//if the object is emitting and hasn't collided yet. proceed for either
		if (Obj1->IsEmittingRight() && !Obj1->GetCollidedRight() || Obj1->IsEmittingLeft() && !Obj1->GetCollidedLeft())
		{
			bool potentialInterupt;
			//also need to check vs terrain if it hits needs to truncate the draw distance of the emitter line.
			//need flag for both.
			if (Obj1->rightTrigger)
			{
				flag = 1;
				potentialInterupt = DoCalculations(Obj1, flag);

			}
			if (Obj1->leftTrigger)
			{
				flag = 2;
				potentialInterupt = DoCalculations(Obj1, flag);

			}

			LineCollider* Line = object2.Has(LineCollider);

			//we do not care if it has a line collider if it doesn't have an emitter, proceed to leave early Will allow for checkpoints later with the linecollider.
			//in other words behavior is in here
			if (object2.Has(Emitter) != nullptr)
			{
				//do collision calculations

				Emitter* ToEffect = object2.Has(Emitter);
				

				if (LineToLineCollision(Obj1,object2, flag))
				{
					//calculate which direction using dot product
					//lineColliders consist of 2 points that make a line. point one should be the left most point.
					gfxVector2 tempVector = *Line->GetPosition1() - *Line->GetPosition2();
					float leftRight = 0.0f;

					if (flag == 1)
					{
						leftRight = gfxVector2::DotProduct(*Obj1->GetDirectionRight(), tempVector);
					}
					else if (flag == 2)
					{
						leftRight = gfxVector2::DotProduct(*Obj1->GetDirectionLeft(), tempVector);
					}

					if (leftRight > 0.0f) // greater than less than 0 for right side collision
					{
						//collides
						Obj1->SetCollidedRight(true);
						ToEffect->SetEmittingRight(true);
						Line->CollidedRight(Obj1);
						ToEffect->SetRightTrigger(true);
					}
					else /* must be side left */
					{
						Obj1->SetCollidedLeft(true);
						ToEffect->SetEmittingLeft(true);
						Line->CollidedLeft(Obj1);
						ToEffect->SetLeftTrigger(true);//placeholding unsure of left right
					}

				}

			}
		}

		return; // cut out quickly if the 1st is true but anything else does not then leave early.
	}

	//the 2nd object is the one emitting
	if (object2.Has(Emitter) && object1.Has(LineCollider))
	{
		//copy the stuff from uptop
	}


	//potentially more stuff for line colliders maybe break this up into different behaviors.
	
}

void Emitter::SetEmitPositionEndR(float x, float y)
{
	emitpositionEndR->x = x;
	emitpositionEndR->y = y;
}
void Emitter::SetEmitPositionEndL(float x, float y)
{
	emitpositionEndL->x = x;
	emitpositionEndL->y = y;
}

void Emitter::SetPositionRight(float x, float y) { emitPositionRight->x = x; emitPositionRight->y = y; };
void Emitter::SetPositionLeft(float x, float y) { emitPositionLeft->y = y; emitPositionLeft->x = x; };