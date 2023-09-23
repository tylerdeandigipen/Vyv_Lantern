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
#include "Math.h"
#include "Vector.h"
#include "ImageBuffer.h"

//BaseSystem::BaseSystem(const char* _name) : name(_name) {}


 ImageBuffer* RenderLightingPass(ImageBuffer* outputBuffer, ImageBuffer* inputBuffer, Light* lightSource[], int totalLights)
{
    for (int i = 0; i < totalLights; ++i)
    {
        float radialFalloff = 0;
        float angularFalloff = 0;
        float angle = 0;
        float lightMultiplier = 0;
        float midAngle = 0;
        float maxAng = 0;
        float minAng = 0;
        float distFromCenter = 0;
        float volLightMultiplier = 0;
        int x = 0;
        int y = 0;
        for (x = 0; x < inputBuffer->size.x; ++x)
        {
            for (y = 0; y < inputBuffer->size.y; ++y)
            {
                angularFalloff = 0;
                radialFalloff = 0;

                distFromCenter = distance(lightSource[i]->position.x, lightSource[i]->position.y, (float)x, (float)y); //find distance from the center of the light   

                if (lightSource[i]->angularWeight != 0)
                {
                    if (lightSource[i]->maxAngle + lightSource[i]->angle > 360)
                    {
                        lightSource[i]->angle -= 360;
                    }
                    else if (lightSource[i]->maxAngle + lightSource[i]->angle < 0)
                    {
                        lightSource[i]->angle += 360;
                    }

                    angle = atan2(x - lightSource[i]->position.x, y - lightSource[i]->position.y) * 57.295779f; //Find angle from point to center relative to x axis, magic number is 180 / pi
                    maxAng = lightSource[i]->maxAngle + lightSource[i]->angle; //rotate relitive angles to be world space angles
                    minAng = lightSource[i]->minAngle + lightSource[i]->angle;
                    angularFalloff = 0;

                    //ajust angle to fit the sign of the input
                    if (angle > 0 && maxAng < 0)
                    {
                        angle -= 360;
                    }
                    if (angle < 0 && minAng > 0)
                    {
                        angle += 360;
                    }

                    if (angle >= minAng && angle <= maxAng)
                    {
                        midAngle = (minAng + maxAng) / 2;
                        angularFalloff = -1 * (((abs(angle - midAngle) - lightSource[i]->maxAngle)) / (lightSource[i]->maxAngle - lightSource[i]->minAngle));
                    }
                }
                else
                {
                    angularFalloff = 1;
                }
                if (lightSource[i]->radialWeight != 0)
                {
                    //calculate radialfalloff
                    radialFalloff = 1 / (1 + (lightSource[i]->radialMult1 * distFromCenter) + (lightSource[i]->radialMult2 * (distFromCenter * distFromCenter)));
                }
                else
                {
                    radialFalloff = 1;
                }
                //clamp both falloffs
                radialFalloff = clamp(lightSource[i]->radialWeight * radialFalloff, 0, 1);
                angularFalloff = clamp(lightSource[i]->angularWeight * angularFalloff, 0, 1);

                //Calculate objects light

                lightMultiplier = lightSource[i]->intensity * radialFalloff * angularFalloff;
                outputBuffer->buffer[x][y] += (inputBuffer->buffer[x][y] * ((lightSource[i]->color * lightMultiplier) / 255));

                lightMultiplier *= lightSource[i]->volumetricIntensity;
                outputBuffer->buffer[x][y] += (lightSource[i]->color * lightMultiplier);


            }
        }
    }
	return inputBuffer;
};
