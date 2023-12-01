//------------------------------------------------------------------------------
//
// File Name:	Light.cpp
// Author(s):	
// Purpose:		Light class implementation for handling light components.
//
// Copyright © 2023 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------
#include "Light.h"
#include "FileIO.h"
#include "Entity.h"
#include "Renderer.h"

Light::Light(const Light& rhs) : Component(rhs)
{
	Type = rhs.Type;

	position = rhs.position;
	leftAnglePos = rhs.leftAnglePos;
	rightAnglePos = rhs.rightAnglePos;

	color = rhs.color;
	minAngle = rhs.minAngle;
	maxAngle = rhs.maxAngle;
	angle = rhs.angle;
	intensity = rhs.intensity;
	radius = rhs.radius;
	radialFalloff = rhs.radialFalloff;
	radialWeight = rhs.radialWeight;
	angularWeight = rhs.angularWeight;
	frustumWeight = rhs.frustumWeight;
	volumetricIntensity = rhs.volumetricIntensity;

    memcpy(Attenuation, rhs.Attenuation, sizeof(Attenuation));
}
Light::Light() : Component(Component::cLight)
{
	minAngle = 0;
	maxAngle = 0;
	angle = 0;
	intensity = 0;
	radius = 0;
	radialFalloff = 0;
	radialWeight = 0;
	angularWeight = 0;
	frustumWeight = 0.15f;
	volumetricIntensity = 0;

    Attenuation[0] = 0.0f;
    Attenuation[1] = 1.0f;
    Attenuation[2] = 1.0f;
}

Component* Light::Clone() const
{
	return new Light(*this);
}

void Light::Update(float dt)
{
	UNREFERENCED_PARAMETER(dt);
}

void Light::Read(json jsonData)
{
	FileIO::GetInstance()->ReadLight(Parent()->lightFile, *this);
	Renderer::GetInstance()->AddLight(*this);

}

std::string Light::TheName()
{
	return "Light";
}

//std::string Light::Name()
//{
//	return "Light";
//}

//std::string Light::GetName()
//{
//	return std::string();
//}

