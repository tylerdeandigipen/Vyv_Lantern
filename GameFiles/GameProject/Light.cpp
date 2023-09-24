#include "Light.h"

Light::Light(const Light& rhs)
{
	position = rhs.position;
	color = rhs.color;
	minAngle = rhs.minAngle;
	maxAngle = rhs.maxAngle;
	angle = rhs.angle;
	intensity = rhs.intensity;
	radialMult1 = rhs.radialMult1;
	radialMult2 = rhs.radialMult2;
	radialWeight = rhs.radialWeight;
	angularWeight = rhs.angularWeight;
	volumetricIntensity = rhs.volumetricIntensity;
}
Light::Light()
{
	minAngle = 0;
	maxAngle = 0;
	angle = 0;
	intensity = 0;
	radialMult1 = 0;
	radialMult2 = 0;
	radialWeight = 0;
	angularWeight = 0;
	volumetricIntensity = 0;
}