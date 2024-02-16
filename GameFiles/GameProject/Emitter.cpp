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
	endpoint = new gfxVector2();
	direction = new gfxVector2();
	distance = 0.0f;
	isEmitting = false;
	isSource = false;
	hit = false;
	isDirty = true;
}

Emitter::Emitter(Emitter const& emitter2cpy) : Component(emitter2cpy)
, isDirty(emitter2cpy.isDirty), isEmitting(emitter2cpy.isEmitting), distance(emitter2cpy.distance)
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
	/*
		
		note:
		1,0 right
		0,1 down
		-1,0 left
		0,-1 up
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

		endpoint -> x = GetPosition().x;
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
	if (isDirty)
	{
		//update the correct system
		//update other things too if needed
		Renderer* pointyBoi = Renderer::GetInstance();
		pointyBoi->laserPoints1[laserReference] = *position;
		pointyBoi->laserPoints2[laserReference] = *endpoint;

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

	Renderer*	pointyBoi = Renderer::GetInstance();


	gfxVector2 compare = { 0,0 };
	//if direction then compare

	if (obj->GetDirection().x > 0)
	{
		compare = pointyBoi->CheckLineForObjects(obj->GetPosition().x, obj->GetPosition().y,
			obj->GetPosition().x + obj->GetDistance(), obj->GetPosition().y);
	}
	else if (obj->GetDirection().y > 0)
	{
		compare = pointyBoi->CheckLineForObjects(obj->GetPosition().x, obj->GetPosition().y,
			obj->GetPosition().x, obj->GetPosition().y + obj->GetDistance());
	}
	else if (obj->GetDirection().x < 0)
	{
		compare = pointyBoi->CheckLineForObjects(obj->GetPosition().x, obj->GetPosition().y,
			obj->GetPosition().x - obj->GetDistance(), obj->GetPosition().y);
	}
	else if (obj->GetDirection().y < 0)
	{
		compare = pointyBoi->CheckLineForObjects(obj->GetPosition().x, obj->GetPosition().y,
			obj->GetPosition().x, obj->GetPosition().y - obj->GetDistance());
	}
	
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
			Vector2 breakLaser = { 9.11, 9.11 };
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
				bool struckShadow = DoCalculations(laser);
				float inbetween;
				//between x values
				if (laser->GetPosition().x >= line->GetPosition1()->x && laser->GetPosition().x <= line->GetPosition2()->x)
				{

					//doesn't matter matter if it doesn't strike will set to 1st wall struck

					if (tempDir.y > 0)
					{
						if (laser->GetPosition().y < lineEmitter->GetPosition().y)
						{

							//inbetween = lineEmitter->GetPosition().y - laser->GetPosition().y;
							// aboce is potential distance between mirros
							// 
							//simple recalculations


							if (struckShadow && lineEmitter->GetPosition().y < laser->GetEndpoint().y)
							{

								//compare the emitter position against the  current end position
								// if it is '>' or '<' depending in direction then do not truncate
								laser->SetEndpoint(lineEmitter->GetPosition());
								lineEmitter->SetEmitting(true);
							}
							else
							{
								//if (inbetween > laser->GetDistance())
								//{
								//	Vector2 BrokenLaser = { 404.1,404.1 };
								//	laser->SetEndpoint(BrokenLaser);
								//	return;
								//}

								laser->SetEndpoint(lineEmitter->GetPosition());
								lineEmitter->SetEmitting(true);
							}


						}
					}
					else if (tempDir.y < 0)
					{
						if (laser->position->y > lineEmitter->GetPosition().y)
						{
							//does collide do thing
														//simple recalculations
							if (struckShadow && lineEmitter->GetPosition().y < laser->GetEndpoint().y)
							{
								//compare the emitter position against the  current end position
								// if it is '>' or '<' depending in direction then do not truncate
								laser->SetEndpoint(lineEmitter->GetPosition());
								lineEmitter->SetEmitting(true);
							}
							else
							{
								laser->SetEndpoint(lineEmitter->GetPosition());
								lineEmitter->SetEmitting(true);
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
							}
							//does collide do thing


						}
					}
				}
				//turn of code for laser look somethingl like this make endpoint->x position->x+1f and should stop rendering the laser.

			}
			else
			{

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
