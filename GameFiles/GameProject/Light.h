#pragma once
#ifndef LIGHT_H
#define LIGHT_H

#include "Color.h"
#include "Vector.h"

enum light_source_type
{
    LightSourceType_Point,
    LightSourceType_Directional,
    LightSourceType_EnumCount
};

class Light
{
public:
	Light();
	Light(const Light &rhs);

    enum light_source_type Type = LightSourceType_Point;
    
    gfxVector2 position;
	Color color;

    int isStatic = 0;
	int isBaked = 0;

	gfxVector2 leftAnglePos;
	gfxVector2 rightAnglePos;

    float minAngle = 0;
	float maxAngle = 0;
	float angle = 0;
	float intensity = 2;
	float radialMult1 = 0.4f;
	float radialMult2 = 0;
	float radialWeight = 0;
	float angularWeight = 1;
	float volumetricIntensity = 0;

    float Attenuation[3];
private:
};


#endif 
