//------------------------------------------------------------------------------
//
// File Name:	TBDLasers.h
// Author(s):	Tyler Dean
// Purpose:		laser header file
//
// Copyright © 2023 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------
#pragma once
#ifndef TBDLasers_H
#define TBDLasers_H
#include <cstdint>
#include "Math.h"
#include "Color.h"
#include "Vector.h"
#define MAX_LASERS 30
#define MAX_MIRRORS 30
#define MAX_CHECKPOINTS 20
struct Laser
{
	// laser must always be on a single axis
	Vector2 pos{ 0,0 };
	Vector2 dir{ 0,0 };
	Color color{0,0,0,0};
	//lasers are turned off by default 
	bool isEmiting = false;
};

struct CheckPoint
{
	// each set must always be on a single axis
	Vector2 xPos1{ 0,0 };
	Vector2 xPos2{ 0,0 };
	Vector2 yPos1{ 0,0 };
	Vector2 yPos2{ 0,0 };

	//set to true to stop laser from passing through checkpoint
	bool isSolid = false;
	//Color required to activate, leave blank for any color to work
	Color requiredColor{ 0,0,0,0 };
	bool isActivated = false;
};

struct Mirror
{
	// each set must always share an axis
	Vector2 xPos1{ 0,0 };
	Vector2 xPos2{ 0,0 };
	Vector2 yPos1{ 0,0 };
	Vector2 yPos2{ 0,0 };

	Vector2 reflectDir{ 0,0 };
	//not implemented yet
	bool reflectFromLeft = 0;
	bool reflectFromRight = 0;
	//keep as zero to inherit the color laser that hits the mirror
	Color overwriteColor{ 0,0,0,0 };
	//do not modify
	bool isActivated1 = 0;
	bool isActivated2 = 0;

	Laser reflectedLaser1;
	Laser reflectedLaser2;
};

class TBDLasers
{
public:
	TBDLasers();
	void Clear();

	//returns true if succesful
	bool AddLaser(Laser* laser);
	bool AddMirror(Mirror* mirror);
	bool AddCheckPoint(CheckPoint* checkPoint);

	Vector2 CheckCollision(int laserIndex);
	void UpdateLasers();

	bool isSolved = false;

	int numLasers = 0;
	Laser* lasers[MAX_LASERS];

	int numMirrors = 0;
	Mirror* mirrors[MAX_MIRRORS];

	int numCheckPoints = 0;
	CheckPoint* checkPoints[MAX_CHECKPOINTS];

private:
	void SetReflectedLaserValuesX(Laser* laser, Mirror* mirror, Vector2 startPos);
	void SetReflectedLaserValuesY(Laser* laser, Mirror* mirror, Vector2 startPos);

};

#endif 
