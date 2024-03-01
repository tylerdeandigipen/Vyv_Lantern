#include "Emitter.h"
#include "Vector.h"
#include "Transform.h"
#include "Entity.h"
#include "LineCollider.h"
#include "Renderer.h"
#include "Logging.h"



Emitter::Emitter() : Component(Component::cEmitter)
{
	hitbyLaser = 0;
	position = new gfxVector2();
	endpoint = new gfxVector2();
	direction = new gfxVector2();
	distance = 0.0f;
	isEmitting = false;
	isSource = false;
	hit = false;
	isDirty = true;
	laserReference = 0;
}

Emitter::Emitter(Emitter const& emitter2cpy) : Component(emitter2cpy)
, isDirty(emitter2cpy.isDirty), isEmitting(emitter2cpy.isEmitting), distance(emitter2cpy.distance)
{
	position = new gfxVector2(*emitter2cpy.position);
	direction = new gfxVector2(*emitter2cpy.direction);
	endpoint = new gfxVector2(*emitter2cpy.endpoint);
	distance = 0.0f;
	isEmitting = false;
	isSource = false;
	hit = false;
	isDirty = true;
	laserReference = 0;
	hitbyLaser = 0;
	//Data ONLY gets set in the Read function.
}

Emitter::~Emitter()
{
	delete position;
	delete direction;
	delete endpoint;
	Renderer::GetInstance()->numLasers = 0;

}

Component* Emitter::Clone() const
{
	return new Emitter(*this);
}

void Emitter::Read(json jsonData)
{
	/*

		note:
		1,0 right
		0,1 down
		-1,0 left
		0,-1 up
	{
	  "Type": "Emitter",
	  "LaserDirection": {
		  "x": 0,
		  "y": 0
	  },
	  "EmitterStart": {
		  "x": 50,
		  "y": 0
	  },
	  "EmitterEndPoint": {
		  "x": 0,
		  "y": 0
	  },
	  "distance": 100,
	  "isSource": true
	}

	*/


	if (jsonData["LaserDirection"].is_object())
	{
		json dir = jsonData["LaserDirection"];
		direction->x = dir["x"];
		direction->y = dir["y"];

	}

	if (jsonData["EmitterStart"].is_object())
	{
		json pos = jsonData["EmitterStart"];
		position->x = pos["x"];
		position->y = pos["y"];
	}

	if (jsonData["EmitterEndPoint"].is_object())
	{
		//will often pass an illegitemate point so that nothing will draw.
		json pos = jsonData["EmitterEndPoint"];
		endpoint->x = pos["x"];
		endpoint->y = pos["y"];
	}

	if (jsonData["distance"] > 0)
	{
		distance = jsonData["distance"];
	}

	if (jsonData["isSource"].is_boolean())
	{
		isSource = jsonData["isSource"];
		isEmitting = isSource;
	}


	if (GetDirection().x > 0)
	{

		endpoint->x = GetPosition().x + GetDistance();
		endpoint->y = GetPosition().y;
	}
	else if (GetDirection().y > 0)
	{

		endpoint->x = GetPosition().x;
		endpoint->y = GetPosition().y + GetDistance();
	}
	else if (GetDirection().x < 0)
	{

		endpoint->x = GetPosition().x - GetDistance();
		endpoint->y = GetPosition().y;
	}
	else if (GetDirection().y < 0)
	{

		endpoint->x = GetPosition().x;
		endpoint->y = GetPosition().y - GetDistance();
	};


	Renderer* renderer = Renderer::GetInstance();
	if (renderer)
	{
		laserReference = renderer->numLasers;
		//will need to see if 
		renderer->laserPoints1[laserReference] = *position;
		renderer->laserPoints2[laserReference] = *endpoint;
		renderer->numLasers++;
	}

	isDirty = true;

}

void Emitter::Update(float dt)
{
	UNREFERENCED_PARAMETER(dt);
	if (isDirty)
	{
		//update the correct system
		//update other things too if needed such as it's position if it moves... but it should be handled by others?
		//this is the only relevant data
		Renderer* pointyBoi = Renderer::GetInstance();
		pointyBoi->laserPoints1[laserReference] = *position;
		pointyBoi->laserPoints2[laserReference] = *endpoint;

		isDirty = false;
	}

	//if (hit && !isSource)
	//{
	//	Renderer* pointyBoi = Renderer::GetInstance();
	//	if ((pointyBoi->laserPoints1[laserReference]) == (pointyBoi->laserPoints2[hitbyLaser]))
	//	{
	//		// the laser that was pointing at it is still pointing at it
	//	}
	//	else
	//	{

	//		hit = false;
	//		Vector2 breakLaser = { 9.11, 9.11 };
	//		SetEndpoint(breakLaser);
	//		isEmitting = false;
	//	}
	//}
	//

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
inline bool LineToLineCollision(Emitter* laser, Entity& line, int flag, bool interuptflag)
{
	UNREFERENCED_PARAMETER(laser);
	UNREFERENCED_PARAMETER(line);
	UNREFERENCED_PARAMETER(flag);
	UNREFERENCED_PARAMETER(interuptflag);
}

/*
* this funciton checks whetehr or not the emitted line will strike anything that casts a shadow.
* flag is a number gives right or left emitter status
* 1 = right
* 2 = left
*/
inline bool DoCalculations(Emitter* obj)
{

	//Renderer* pointyBoi = Renderer::GetInstance();


	gfxVector2 compare = { 0,0 };
	//if direction then compare
	/*
	if (obj->GetDirection().x > 0)
	{
		compare = pointyBoi->LaserCheckLineForObject(obj->GetPosition().x, obj->GetPosition().y,
			obj->GetPosition().x + obj->GetDistance(), obj->GetPosition().y);
	}
	else if (obj->GetDirection().y > 0)
	{
		compare = pointyBoi->LaserCheckLineForObject(obj->GetPosition().x, obj->GetPosition().y,
			obj->GetPosition().x, obj->GetPosition().y + obj->GetDistance());
	}
	else if (obj->GetDirection().x < 0)
	{
		compare = pointyBoi->LaserCheckLineForObject(obj->GetPosition().x, obj->GetPosition().y,
			obj->GetPosition().x - obj->GetDistance(), obj->GetPosition().y);
	}
	else if (obj->GetDirection().y < 0)
	{
		compare = pointyBoi->LaserCheckLineForObject(obj->GetPosition().x, obj->GetPosition().y,
			obj->GetPosition().x, obj->GetPosition().y - obj->GetDistance());
	}
	*/
	Vector2 end = obj->GetEndpoint();

	if (compare.operator == (obj->GetEndpoint()))
	{
		return false;
	}
	else
	{
		// then we set endpoint too compare
		obj->SetEndpoint(compare);
		return true;
	}




}

void Emitter::EmitterCollisionHandler(Entity& object1, Entity& object2)
{


	//this may be terrible currently ignores terrain
	//additionally does not have a way to turn off... will need to group source to fix issue can't think of how off the top of head.
	if (object1.Has(Emitter) && object2.Has(LineCollider))
	{
		//potential turn off code
		//if laser collider ishit and 

		Emitter* laser = object1.Has(Emitter);

		if (!laser->isEmitting)
		{
			//for rendering code
			Vector2 breakLaser = { 9.11f, 9.11f };
			laser->SetEndpoint(breakLaser);
			return;
		}


		bool struckShadow = DoCalculations(laser);


		//this is a  single emitter case may need to adjust for future ifor mulit emitter cases
		if (object2.Has(Emitter))
		{

			LineCollider* line = object2.Has(LineCollider); //note that position one should be the left most and position 2 should be the right most

			Emitter* lineEmitter = object2.Has(Emitter);
			gfxVector2 tempDir = laser->GetDirection();

			float inbetween; // check if  mirror is too far
			//between x values
			if (laser->GetPosition().x >= line->GetPosition1()->x && laser->GetPosition().x <= line->GetPosition2()->x)
			{

				//doesn't matter matter if it doesn't strike will set to 1st wall struck

				if (tempDir.y > 0)
				{
					if (laser->GetPosition().y < lineEmitter->GetPosition().y)
					{



						if (struckShadow && lineEmitter->GetPosition().y < laser->GetEndpoint().y)
						{

							//compare the emitter position against the  current end position
							// if it is '>' or '<' depending in direction then do not truncate
							laser->SetEndpoint(lineEmitter->GetPosition());
							lineEmitter->SetEmitting(true);
							//lineEmitter->SetHit(true, laser->laserReference);
						}
						else
						{

							inbetween = lineEmitter->GetPosition().y - laser->position->y;
							//does not reach
							if (inbetween > laser->GetDistance())
							{
								return;
							}


							laser->SetEndpoint(lineEmitter->GetPosition());
							lineEmitter->SetEmitting(true);
							//lineEmitter->SetHit(true, laser->laserReference);
						}


					}
				}
				else if (tempDir.y < 0)
				{
					if (laser->position->y > lineEmitter->GetPosition().y)
					{
					
						//now the endpoint should be the 1st point which the shadow is hit.
						if (struckShadow && lineEmitter->GetPosition().y < laser->GetEndpoint().y)
						{
							//compare the emitter position against the  current end position
							// if it is '>' or '<' depending in direction then do not truncate
							//case may be wrong?
							laser->SetEndpoint(lineEmitter->GetPosition());
							lineEmitter->SetEmitting(true);
							//lineEmitter->SetHit(true, laser->laserReference);
						}
						else
						{
							inbetween = laser->position->y - lineEmitter->GetPosition().y;

							//does not reach
							if (inbetween > laser->GetDistance())
							{
								return;
							}

							laser->SetEndpoint(lineEmitter->GetPosition());
							lineEmitter->SetEmitting(true);
							//lineEmitter->SetHit(true, laser->laserReference);
						}
					}
				}



			}

			//between y values
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
							laser->SetEndpoint(lineEmitter->GetPosition());
							lineEmitter->SetEmitting(true);
							//lineEmitter->SetHit(true, laser->laserReference);
						}
						else
						{
							inbetween = laser->position->x - lineEmitter->GetPosition().x;
							//does not reach
							if (inbetween > laser->GetDistance())
							{
								return;
							}

							laser->SetEndpoint(lineEmitter->GetPosition());
							lineEmitter->SetEmitting(true);
							//lineEmitter->SetHit(true, laser->laserReference);
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
							laser->SetEndpoint(lineEmitter->GetPosition());
							lineEmitter->SetEmitting(true);
							//lineEmitter->SetHit(true, laser->laserReference);
						}
						else
						{
							inbetween = lineEmitter->GetPosition().x - laser->position->x;
							//does not reach
							if (inbetween > laser->GetDistance())
							{
								return;
							}

							laser->SetEndpoint(lineEmitter->GetPosition());
							lineEmitter->SetEmitting(true);
							//lineEmitter->SetHit(true, laser->laserReference);
						}


					}
				}
			}
			//turn of code for laser look somethingl like this make endpoint->x position->x+1f and should stop rendering the laser.

		}
		else
		{
			// other behaviors





			//come here for other behaviors i guess.
			return;
		}


		//the 2nd object is the one emitting
		if (object2.Has(Emitter) && object1.Has(LineCollider))
		{
			//copy the stuff from uptop
		}

	}
	//potentially more stuff for line colliders maybe break this up into different behaviors.

}



void Emitter::SetPosition(Vector2 SetP)
{
	position->x = SetP.x;
	position->y = SetP.y;
	isDirty = true;
}

/*
* for behavior switch mirror you will want to call this.
*/
void Emitter::SetDirection(Vector2 SetP)
{
	direction->x = SetP.x;
	direction->y = SetP.y;
	isDirty = true;
}
void Emitter::SetEndpoint(Vector2 SetP)
{
	endpoint->x = SetP.x;
	endpoint->y = SetP.y;
	isDirty = true;
}
