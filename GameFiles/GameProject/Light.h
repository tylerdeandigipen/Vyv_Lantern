#pragma once
#ifndef LIGHT_H
#define LIGHT_H

#include "Color.h"
#include "Vector.h"

class Light
{
public:
	gfxVector2 position;
	Color color;
	float minAngle;
	float maxAngle;
	float intensity;
	float radialMult1;
	float radialMult2;
	float radialWeight;
	float angularWeight;
	float volumetricIntensity;
private:
};


#endif 