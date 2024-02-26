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
#include "Vector.h"
#define MAX_LASERS 20
#define MAX_MIRRORS 20

struct Laser
{
	// laser must always be on a single axis
	Vector2 pos{ 0,0 };
	Vector2 dir{ 0,0 };
	//lasers are turned off by default 
	bool isEmiting = false;
};

struct Mirror
{
	// mirror must always share an axis
	Vector2 pos1{ 0,0 };
	Vector2 pos2{ 0,0 };
	Vector2 reflectDir{ 0,0 };
	//not implemented yet
	bool reflectFromLeft = 0;
	bool reflectFromRight = 0;
	
	//do not modify
	bool isActivated = 0;
	Laser reflectedLaser;
};

class TBDLasers
{
public:
	TBDLasers();

	//returns true if succesful
	bool AddLaser(Laser* laser);
	bool AddMirror(Mirror* mirror);

	Vector2 CheckCollision(int laserIndex);
	void UpdateLasers();

	int numLasers;
	Laser* lasers[MAX_LASERS];

	int numMirrors;
	Mirror* mirrors[MAX_LASERS];
};

#endif 
