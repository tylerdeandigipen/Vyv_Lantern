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
#include "Vector"

//BaseSystem::BaseSystem(const char* _name) : name(_name) {}

struct Color
{
	int r;
	int g;
	int b;
	int a;
};

struct Light
{
	struct Vector2 position;
	struct Color color;
	float minAngle;
	float maxAngle;
	float intensity;
	float radialMult1;
	float radialMult2;
};

struct Vector2
{
	int x;
	int y;
};

struct ImgBuffer
{
	struct Vector2 size;
	struct Color buffer_[];
};

const int ScreenSizeX = 240;
const int ScreenSizeY = 135;
ImgBuffer* CreateImgBuffer()
{
	ImgBuffer* lightBuffer = (ImgBuffer*)calloc(1, sizeof *lightBuffer + sizeof(Color[ScreenSizeX][ScreenSizeY]));
	lightBuffer->size.x = ScreenSizeX;
	lightBuffer->size.y = ScreenSizeY;
	return lightBuffer;
};

//forward def because not made yet
float Distance(int x1, int y1, int x2, int y2);
float clamp(float x, float max, float min);

ImgBuffer* RenderLightingPass(ImgBuffer *lightBuffer, Light *lightSource)
{
	Color(*buffer)[ScreenSizeX] = (Color(*)[ScreenSizeX]) lightBuffer->buffer_;
	struct Color temp;
	buffer[1][2] = temp;

	float radialFalloff;
	float angularFalloff;
	float angle;
	for (int x = 0; x < lightBuffer->size.x; x++)
	{
		for (int y = 0; y < lightBuffer->size.y; y++)
		{
			float intenstity;
			float distance = Distance(x, y, lightSource->position.x, lightSource->position.y);
			angle = atan2(y - lightSource->position.y, x - lightSource->position.x);
			if (angle > 0 && lightSource->maxAngle < 0)
			{
				angle -= 360;
			}
			if (angle < 0 && lightSource->minAngle > 0)
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

			radialFalloff = 1.0 / (1.0f + lightSource->radialMult1 * distance + lightSource->radialMult2 * pow(distance, 2));

			clamp(angularFalloff, 5, 0);
			clamp(radialFalloff, 5, 0);

			float lightIntensity = radialFalloff * angularFalloff * lightSource->intensity;
			buffer[x][y] = (buffer[x][y] * lightIntensity) + ((lightSource->color / 255) * lightIntensity);
			s
		}
	}

};