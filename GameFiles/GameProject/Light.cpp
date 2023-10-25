#include "Light.h"

Light::Light(const Light& rhs)
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
Light::Light()
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
