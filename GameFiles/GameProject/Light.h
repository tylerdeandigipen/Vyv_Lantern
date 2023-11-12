#pragma once
#ifndef LIGHT_H
#define LIGHT_H

#include "Color.h"
#include "Component.h"
#include "Vector.h"

#include <string>

enum light_source_type
{
    LightSourceType_Point,
    LightSourceType_Directional,
    LightSourceType_EnumCount
};
using string = std::string;
class Light : public Component 
{
public:
	Light();
	Light(const Light &rhs);

	Component* Clone() const override;
	void Update(float dt) override;
	void Read(json jsonData) override;
    gfxVector2 position;
	Color color;

	gfxVector2 leftAnglePos;
	gfxVector2 rightAnglePos;

	enum light_source_type Type = LightSourceType_Point;
    float minAngle = 0;
	float maxAngle = 0;
	float angle = 0;
	float intensity = 1;
	float radius = 100;
	float radialFalloff = 1;
	float radialWeight = 0;
	float angularWeight = 1;
	float frustumWeight = 0.15f;
	float volumetricIntensity = 0;

    float Attenuation[3];
private:
};


#endif 
