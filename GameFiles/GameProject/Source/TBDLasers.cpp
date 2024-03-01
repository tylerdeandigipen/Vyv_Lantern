//------------------------------------------------------------------------------
//
// File Name:	TBDLasers.cpp
// Author(s):	Tyler Dean
// Purpose:		laser implementation
//
// Copyright © 2023 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------
#include "TBDLasers.h"
#include "Renderer.h"
#include "Math.h"

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

bool TBDLasers::AddCheckPoint(CheckPoint* checkPoint)
{
	if (numCheckPoints < MAX_CHECKPOINTS)
	{
		for (int i = 0; i < numCheckPoints; i++)
		{
			if (checkPoints[i] == checkPoint)
			{
				return false;
			}
		}
		checkPoints[numLasers] = checkPoint;
		numCheckPoints += 1;
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
	// temp stuff can delete 
	lasers[0]->color = Color{ 84,0,255,255 };
	if (tempThingLol == 0)
	{
		checkPoints[0] = new CheckPoint();
		checkPoints[0]->xPos1 = Vector2{ 126, 190 };
		checkPoints[0]->xPos2 = Vector2{ 106, 190 };

		checkPoints[0]->yPos1 = Vector2{ 116, 200 };
		checkPoints[0]->yPos2 = Vector2{ 116, 180 };

		checkPoints[1] = new CheckPoint();
		checkPoints[1]->xPos1 = Vector2{ 186, 168 };
		checkPoints[1]->xPos2 = Vector2{ 166, 168 };

		checkPoints[1]->yPos1 = Vector2{ 176, 158 };
		checkPoints[1]->yPos2 = Vector2{ 176, 178 };
		checkPoints[1]->isSolid = true;

		numCheckPoints = 2;
		tempThingLol = 1;
	}
	
	for (int i = 0; i < numMirrors; i++)
	{
		if (mirrors[i]->isActivated == false)
		{
			mirrors[i]->reflectedLaser.isEmiting = false;
		}
		mirrors[i]->isActivated = false;
	}

	bool isSolved = true;
	for (int i = 0; i < numCheckPoints; i++)
	{
		if (checkPoints[i]->isActivated == false)
		{
			isSolved = false;
		}
		checkPoints[i]->isActivated = false;
	}

	for (int i = 0; i < numLasers; i++)
	{
		renderer->numLasers = numLasers;
		renderer->laserPoints1[i] = lasers[i]->pos - renderer->GetCameraPosition(); 
		renderer->laserPoints2[i] = CheckCollision(i);
		renderer->laserColor[i] = lasers[i]->color;
		if (renderer->laserPoints2[i].x == 9001 && renderer->laserPoints2[i].y == -9001)//9001 is a code for if the current laser is invalid
		{
			lasers[i]->isEmiting = false;
		}
	}
}

Color defaultColor{ 0,0,0,0 };
Vector2 TBDLasers::CheckCollision(int laserIndex)
{
	if (lasers[laserIndex] == NULL || lasers[laserIndex]->isEmiting == false)
	{
		return Vector2{ 9001,-9001 };
	}

	Vector2 laserPos1 = lasers[laserIndex]->pos - renderer->GetCameraPosition();
	Vector2 laserPos2 = laserPos1 + (lasers[laserIndex]->dir * 10); //10 is an arbitrary constant
	float minDist = 999999;
	//check for mirror collision
	for (int i = 0; i < numMirrors; i++)
	{
		if (mirrors[i] != NULL && &mirrors[i]->reflectedLaser != lasers[laserIndex])
		{
			//check if its along the y axis
			if (lasers[laserIndex]->dir.y != 0)
			{
				Vector2 mirrorPoint1 = mirrors[i]->xPos1 - renderer->GetCameraPosition();
				Vector2 mirrorPoint2 = mirrors[i]->xPos2 - renderer->GetCameraPosition();
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
						Vector2 tempPos2 = renderer->LaserCheckLineForObject(laserPos1, Vector2{ laserPos1.x, mirrorPoint1.y });
						if (abs((tempPos2.y - laserPos1.y) * (tempPos2.y - laserPos1.y)) < minDist)
						{
							laserPos2 = tempPos2;
							minDist = abs((tempPos2.y - laserPos1.y) * (tempPos2.y - laserPos1.y));
							if (laserPos2.y == mirrorPoint1.y)
							{
								if (AddLaser(&mirrors[i]->reflectedLaser))
								{
									mirrors[i]->reflectedLaser.pos = laserPos2 + renderer->GetCameraPosition();
									mirrors[i]->reflectedLaser.dir = Vector2{mirrors[i]->reflectDir.x, 0};
								}
								mirrors[i]->isActivated = true;
								mirrors[i]->reflectedLaser.isEmiting = true;
								if (mirrors[i]->overwriteColor == defaultColor)
								{
									mirrors[i]->reflectedLaser.color = lasers[i]->color;
								}
								else
								{
									mirrors[i]->reflectedLaser.color = mirrors[i]->overwriteColor;
								}
							}
						}
					}
				}
			}
			else if (lasers[laserIndex]->dir.x != 0) //check if its along the x axis
			{
				Vector2 mirrorPoint1 = mirrors[i]->yPos1 - renderer->GetCameraPosition();
				Vector2 mirrorPoint2 = mirrors[i]->yPos2 - renderer->GetCameraPosition();
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
						Vector2 tempPos2 = renderer->LaserCheckLineForObject(laserPos1, Vector2{ mirrorPoint1.x, laserPos1.y });
						if (abs((tempPos2.x - laserPos1.x) * (tempPos2.x - laserPos1.x)) < minDist)
						{
							laserPos2 = tempPos2;
							minDist = abs((tempPos2.x - laserPos1.x) * (tempPos2.x - laserPos1.x));
							if (laserPos2.x == mirrorPoint1.x)
							{
								if (AddLaser(&mirrors[i]->reflectedLaser))
								{
									mirrors[i]->reflectedLaser.pos = laserPos2 + renderer->GetCameraPosition();
									mirrors[i]->reflectedLaser.dir = Vector2{ 0, mirrors[i]->reflectDir.y };
								}
								mirrors[i]->isActivated = true;
								mirrors[i]->reflectedLaser.isEmiting = true;
								if (mirrors[i]->overwriteColor == defaultColor)
								{
									mirrors[i]->reflectedLaser.color = lasers[i]->color;
								}
								else
								{
									mirrors[i]->reflectedLaser.color = mirrors[i]->overwriteColor;
								}
							}
						}
					}
				}
			}
		}
	}


	for (int i = 0; i < numCheckPoints; i++)
	{
		if (checkPoints[i] != NULL)
		{
			//check if its along the y axis
			if (lasers[laserIndex]->dir.y != 0)
			{
				Vector2 mirrorPoint1 = checkPoints[i]->xPos1 - renderer->GetCameraPosition();
				Vector2 mirrorPoint2 = checkPoints[i]->xPos2 - renderer->GetCameraPosition();
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
						Vector2 tempPos2 = renderer->LaserCheckLineForObject(laserPos1, Vector2{ laserPos1.x, mirrorPoint1.y });
						if (abs((tempPos2.y - laserPos1.y) * (tempPos2.y - laserPos1.y)) < minDist && abs((tempPos2.y - laserPos1.y) * (tempPos2.y - laserPos1.y)) <= abs((mirrorPoint1.y - laserPos1.y) * (mirrorPoint1.y - laserPos1.y)))
						{	
							if (checkPoints[i]->isSolid)
							{
								laserPos2 = tempPos2;
								minDist = abs((tempPos2.y - laserPos1.y) * (tempPos2.y - laserPos1.y));
							}
							if (checkPoints[i]->requiredColor == defaultColor || checkPoints[i]->requiredColor == lasers[i]->color)
							{
								checkPoints[i]->isActivated = true;
							}
						}
					}
				}
			}
			else if (lasers[laserIndex]->dir.x != 0) //check if its along the x axis
			{
				Vector2 mirrorPoint1 = checkPoints[i]->yPos1 - renderer->GetCameraPosition();
				Vector2 mirrorPoint2 = checkPoints[i]->yPos2 - renderer->GetCameraPosition();
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
						Vector2 tempPos2 = renderer->LaserCheckLineForObject(laserPos1, Vector2{ mirrorPoint1.x, laserPos1.y });
						if (abs((tempPos2.x - laserPos1.x) * (tempPos2.x - laserPos1.x)) < minDist && abs((tempPos2.x - laserPos1.x) * (tempPos2.x - laserPos1.x)) <= abs((mirrorPoint1.x - laserPos1.x) * (mirrorPoint1.x - laserPos1.x)))
						{
							if (checkPoints[i]->isSolid)
							{
								laserPos2 = tempPos2;
								minDist = abs((tempPos2.x - laserPos1.x) * (tempPos2.x - laserPos1.x));
							}
							if (checkPoints[i]->requiredColor == defaultColor || checkPoints[i]->requiredColor == lasers[i]->color)
							{
								checkPoints[i]->isActivated = true;
							}
						}
					}
				}
			}
		}
	}
	

	if (laserPos1 + (lasers[laserIndex]->dir * 10) == laserPos2) //10 is an arbitrary constant
	{
		laserPos2 = laserPos1 + (lasers[laserIndex]->dir * 300); //300 is abitrary max length
		return renderer->LaserCheckLineForObject(laserPos1, laserPos2);
	}

	return laserPos2;
}

void TBDLasers::Clear()
{
	numLasers = 0;
	numMirrors = 0;
}