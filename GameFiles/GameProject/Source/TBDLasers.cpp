//------------------------------------------------------------------------------
//
// File Name:	TBDLasers.cpp
// Author(s):	Tyler Dean
// Purpose:		laser implementation
//
// Copyright � 2023 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------
#include "TBDLasers.h"
#include "Renderer.h"
Renderer* renderer = Renderer::GetInstance();
TBDLasers::TBDLasers() // default constructor definition
	: numLasers{0},
	lasers{0},
	numMirrors{0},
	mirrors{0}
{
}

bool TBDLasers::AddLaser(Laser* laser)
{
	if (numLasers < MAX_LASERS)
	{
		for (int i = 0; i < numLasers; i++)
		{
			if (lasers[i] == laser)
			{
				return false;
			}
		}
		lasers[numLasers] = laser;
		numLasers += 1;
		return true;
	}
	return false;
}

bool TBDLasers::AddMirror(Mirror* mirror)
{
	if (numMirrors < MAX_MIRRORS)
	{
		for (int i = 0; i < numMirrors; i++)
		{
			if (mirrors[i] == mirror)
			{
				return false;
			}
		}
		mirrors[numMirrors] = mirror;
		numMirrors += 1;
		return true;
	}
	return false;
}

bool tempThingLol = 0;
void TBDLasers::UpdateLasers()
{
	if (tempThingLol == 0)
	{
		Laser* tempLaser = new Laser;
		Mirror* tempMirror = new Mirror;
		Mirror* tempMirror2 = new Mirror;

		tempLaser->pos = Vector2{ 100,100 };
		tempLaser->dir = Vector2{ 1,0 };
		tempLaser->isEmiting = true;

		tempMirror->pos1 = Vector2{ 160,150 };
		tempMirror->pos2 = Vector2{ 160,50 };
		tempMirror->reflectDir = Vector2{ 0,1 };


		tempMirror2->pos1 = Vector2{ 130,130 };
		tempMirror2->pos2 = Vector2{ 190,130 };
		tempMirror2->reflectDir = Vector2{ -1,0 };

		AddLaser(tempLaser);
		AddMirror(tempMirror);
		AddMirror(tempMirror2);

		tempThingLol = 1;
	}

	for (int i = 0; i < numMirrors; i++)
	{
		mirrors[i]->isActivated = false;
	}
	for (int i = 0; i < numLasers; i++)
	{
		renderer->numLasers = numLasers;
		renderer->laserPoints1[i] = lasers[i]->pos - renderer->GetCameraPosition(); 
		renderer->laserPoints2[i] = CheckCollision(i);
		if (renderer->laserPoints2[i].x == 9001)//9001 is a code for if the current laser is invalid
		{
			lasers[i]->isEmiting = false;
		}
	}
}

Vector2 TBDLasers::CheckCollision(int laserIndex)
{
	if (lasers[laserIndex] == NULL || lasers[laserIndex]->isEmiting == false)
	{
		return Vector2{9001,9001};
	}

	Vector2 laserPos1 = lasers[laserIndex]->pos - renderer->GetCameraPosition();
	Vector2 laserPos2 = laserPos1 + (lasers[laserIndex]->dir * 10); //10 is an arbitrary constant

	for (int i = 0; i < numMirrors; i++)
	{
		if (mirrors[i] != NULL && &mirrors[i]->reflectedLaser != lasers[laserIndex])
		{
			Vector2 mirrorPoint1 = mirrors[i]->pos1 - renderer->GetCameraPosition();
			Vector2 mirrorPoint2 = mirrors[i]->pos2 - renderer->GetCameraPosition();

			//check if its along the y axis
			if (lasers[laserIndex]->dir.y != 0)
			{
				//sanitize mirror
				if (mirrorPoint1.x > mirrorPoint2.x)
				{
					Vector2 temp = mirrorPoint1;
					mirrorPoint1 = mirrorPoint2;
					mirrorPoint2 = temp;
				}

				//check if laser x is ever inbetween the two mirror points
				if (laserPos1.x > mirrorPoint1.x && laserPos1.x < mirrorPoint2.x)
				{
					//check if mirror is in the same dir as laser
					if ((lasers[laserIndex]->dir.y > 0 && laserPos1.y < mirrorPoint1.y) || (lasers[laserIndex]->dir.y < 0 && laserPos1.y > mirrorPoint1.y))
					{
						laserPos2 = renderer->LaserCheckLineForObject(laserPos1, Vector2{ laserPos1.x, mirrorPoint1.y });
						if (laserPos2.y == mirrorPoint1.y)
						{
							if (AddLaser(&mirrors[i]->reflectedLaser))
							{
								mirrors[i]->reflectedLaser.pos = laserPos2;
								mirrors[i]->reflectedLaser.dir = mirrors[i]->reflectDir;
							}
							mirrors[i]->isActivated = true;
							mirrors[i]->reflectedLaser.isEmiting = true;
						}
						return laserPos2;
					}
				}
			}
			else if (lasers[laserIndex]->dir.x != 0) //check if its along the x axis
			{
				//sanitize mirror
				if (mirrorPoint1.y > mirrorPoint2.y)
				{
					Vector2 temp = mirrorPoint1;
					mirrorPoint1 = mirrorPoint2;
					mirrorPoint2 = temp;
				}

				//check if laser x is ever inbetween the two mirror points
				if (laserPos1.y > mirrorPoint1.y && laserPos1.y < mirrorPoint2.y)
				{
					//check if mirror is in the same dir as laser
					if ((lasers[laserIndex]->dir.x > 0 && laserPos1.x < mirrorPoint1.x) || (lasers[laserIndex]->dir.x < 0 && laserPos1.x > mirrorPoint1.x))
					{
						laserPos2 = renderer->LaserCheckLineForObject(laserPos1, Vector2{ mirrorPoint1.x, laserPos1.y });
						if (laserPos2.x == mirrorPoint1.x)
						{
							if (AddLaser(&mirrors[i]->reflectedLaser))
							{
								mirrors[i]->reflectedLaser.pos = laserPos2;
								mirrors[i]->reflectedLaser.dir = mirrors[i]->reflectDir;
							}
							mirrors[i]->isActivated = true;
							mirrors[i]->reflectedLaser.isEmiting = true;
						}
						return laserPos2;
					}
				}
			}
		}
	}
	laserPos2 = laserPos1 + (lasers[laserIndex]->dir * 300); //300 is abitrary max length
	return renderer->LaserCheckLineForObject(laserPos1, laserPos2);
}

