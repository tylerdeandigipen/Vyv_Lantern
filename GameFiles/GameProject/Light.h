#pragma once
#ifndef LIGHT_H
#define LIGHT_H

class Color {};

class Light
{
public:
	struct Vector2 position;
	Color color;
	float minAngle;
	float maxAngle;
	float intensity;
	float radialMult1;
	float radialMult2;
private:
};


#endif 