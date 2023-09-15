//------------------------------------------------------------------------------
//
// File Name:	BaseSystem.cpp
// Author(s):	Tyler Dean
// Purpose:		The Renderer implementation
//
// Copyright © 2023 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------
#include "Renderer.h"
#include "Color.h"
#include "Light.h"
#include "Math_Header.h"
#include "Vector.h"
#include "ImageBuffer.h"


const int ScreenSizeX = 240;
const int ScreenSizeY = 135;

//BaseSystem::BaseSystem(const char* _name) : name(_name) {}


//forward def because not made yet
float Distance(float x1, float y1, float x2, float y2);
float clamp(float x, float max, float min);

ImageBuffer* RenderLightingPass(ImageBuffer *lightBuffer, Light *lightSource)
{
	Color temp;
	temp = temp * 2.0f;

	float radialFalloff;
	float angularFalloff;
	float angle;
	for (int x = 0; x < lightBuffer->size.x; x++)
	{
		for (int y = 0; y < lightBuffer->size.y; y++)
		{
			float distance = Distance((float)x, (float)y, lightSource->position.x, lightSource->position.y);
			angle = atan2((float)y - lightSource->position.y, (float)x - lightSource->position.x);

			if (angle > 0.0f && lightSource->maxAngle < 0.0f)
			{
				angle -= 360.0f;
			}
			if (angle < 0.0f && lightSource->minAngle > 0.0f)
			{
				angle += 360;
			}
			angularFalloff = 0;
			if (angle >= lightSource->minAngle && angle <= lightSource->maxAngle)
			{
				float midAngle = lightSource->maxAngle - lightSource->minAngle;
				float tempMax = lightSource->maxAngle - midAngle;
				float tempMin = lightSource->minAngle - midAngle;
				angularFalloff = -1 * (angle - tempMax) / (tempMax - tempMin);
			}

			radialFalloff = 1.0f / (1.0f + lightSource->radialMult1 * distance + lightSource->radialMult2 * (distance * distance));

			clamp(angularFalloff, 5.0f, 0.0f);
			clamp(radialFalloff, 5.0f, 0.0f);

			float lightIntensity = radialFalloff * angularFalloff * lightSource->intensity;
			lightBuffer->buffer[x][y] = (lightBuffer->buffer[x][y] * lightIntensity) + ((lightSource->color / 255.0f) * lightIntensity);
		}
	}
	return lightBuffer;
};