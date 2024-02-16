#include "Emitter.h"
#include "Vector.h"
#include "Transform.h"
#include "Entity.h"
#include "LineCollider.h"
#include "Renderer.h"
#include "Logging.h"

Renderer* pointyBoi = nullptr;

Emitter::Emitter() : Component(Component::cEmitter)
{
	distance = 0;
	position = new gfxVector2();
	endpoint = new gfxVector2();
	direction = new gfxVector2();
}

Emitter::Emitter(Emitter const& emitter2cpy) : Component(emitter2cpy)
, isDirty(emitter2cpy.isDirty), distance(emitter2cpy.distance), isEmitting(emitter2cpy.isEmitting)
{
	position = new gfxVector2(*emitter2cpy.position);
	direction = new gfxVector2(*emitter2cpy.direction);
	endpoint = new gfxVector2(*emitter2cpy.endpoint);
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
	if (isDirty)
	{
		//update the correct system

		isDirty = false;
	}
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
	if (!pointyBoi)
	{
		pointyBoi = Renderer::GetInstance();
	}

	gfxVector2 compare = { 0,0 };
	//if direction then compare

	if (obj->GetDirection().x > 0)
	{
		gfxVector2 compare = pointyBoi->CheckLineForObjects(obj->GetPosition().x, obj->GetPosition().y,
			obj->GetPosition().x + obj->GetDistance(), obj->GetPosition().y);
	}
	else if (obj->GetDirection().y > 0)
	{
		gfxVector2 compare = pointyBoi->CheckLineForObjects(obj->GetPosition().x, obj->GetPosition().y,
			obj->GetPosition().x, obj->GetPosition().y + obj->GetDistance());
	}
	else if (obj->GetDirection().x < 0)
	{
		gfxVector2 compare = pointyBoi->CheckLineForObjects(obj->GetPosition().x, obj->GetPosition().y,
			obj->GetPosition().x - obj->GetDistance(), obj->GetPosition().y);
	}
	else if (obj->GetDirection().y < 0)
	{
		gfxVector2 compare = pointyBoi->CheckLineForObjects(obj->GetPosition().x, obj->GetPosition().y,
			obj->GetPosition().x, obj->GetPosition().y - obj->GetDistance());
	}
	
	if (compare.operator== (obj->GetEndpoint()))
	{
		return false;
	}
	else
	{
		// then we set endpoint too compare
		obj->SetEndpoint(&compare);
		return true;
	}




}

void Emitter::EmitterCollisionHandler(Entity& object1, Entity& object2)
{


	//this may be terrible currently ignores terrain
	//additionally does not have a way to turn off... will need to group source to fix issue can't think of how off the top of head.
	if (object1.Has(Emitter) && object2.Has(LineCollider))
	{
		Emitter* laser = object1.Has(Emitter);
		if (laser->isDirty)
		{
			//this is a  single emitter case may need to adjust for future ifor mulit emitter cases
			if (object2.Has(Emitter))
			{
				LineCollider* line = object2.Has(LineCollider); //note that position one should be the left most and position 2 should be the right most

				Emitter* lineEmitter = object2.Has(Emitter);
				gfxVector2 tempDir = laser->GetDirection();
				bool struckShadow = DoCalculations(laser);


				if (laser->GetPosition().x >= line->GetPosition1()->x && laser->GetPosition().x <= line->GetPosition2()->x)
				{

					//doesn't matter matter if it doesn't strike will set to 1st wall struck

					if (tempDir.y > 0)
					{
						if (laser->GetPosition().y > line->GetPosition1()->y)
						{
							//simple recalculations
							if (struckShadow && lineEmitter->GetPosition().y < laser->GetEndpoint().y)
							{
								//compare the emitter position against the  current end position
								// if it is '>' or '<' depending in direction then do not truncate
								laser->SetEndpoint(&lineEmitter->GetPosition());
							}


						}
					}
					else if (tempDir.y < 0)
					{
						if (laser->position->y < line->GetPosition1()->y)
						{
							//does collide do thing
														//simple recalculations
							if (struckShadow && lineEmitter->GetPosition().y > laser->GetEndpoint().y)
							{
								//compare the emitter position against the  current end position
								// if it is '>' or '<' depending in direction then do not truncate
								laser->SetEndpoint(&lineEmitter->GetPosition());
							}
						}
					}


				}

				if (laser->GetPosition().y >= line->GetPosition1()->y && laser->GetPosition().y <= line->GetPosition2()->x)
				{
					if (tempDir.x > 0)
					{
						if (laser->position->x > line->GetPosition1()->x)
						{
							//does collide	do thing
														//simple recalculations
							if (struckShadow && lineEmitter->GetPosition().x < laser->GetEndpoint().x)
							{
								//compare the emitter position against the  current end position
								// if it is '>' or '<' depending in direction then do not truncate
								laser->SetEndpoint(&lineEmitter->GetPosition());
							}
						}
					}
					else if (tempDir.x < 0)
					{
						if (laser->position->x < line->GetPosition1()->x)
						{
							if (struckShadow && lineEmitter->GetPosition().x > laser->GetEndpoint().x)
							{
								//compare the emitter position against the  current end position
								// if it is '>' or '<' depending in direction then do not truncate
								laser->SetEndpoint(&lineEmitter->GetPosition());
							}
							//does collide do thing


						}
					}
				}
			}
			else
			{

				//come here for other behaviors i guess.
				return;
			}
		}
		else
		{
			return;
		}


	}

	//the 2nd object is the one emitting
	if (object2.Has(Emitter) && object1.Has(LineCollider))
	{
		//copy the stuff from uptop
	}


	//potentially more stuff for line colliders maybe break this up into different behaviors.
	
}



void Emitter::SetPosition(gfxVector2* SetP)
{
	position = SetP;
	isDirty = true;
}

void Emitter::SetDirection(gfxVector2* SetP)
{
	direction = SetP;
	isDirty = true;
}
void Emitter::SetEndpoint(gfxVector2* SetP)
{
	endpoint = SetP;
	isDirty = true;
}
