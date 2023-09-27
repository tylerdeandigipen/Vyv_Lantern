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
#include <SDL/SDL.h>
//BaseSystem::BaseSystem(const char* _name) : name(_name) {}


 void Renderer::RenderLightingPass()
{

     int x = 0;
     int y = 0;

     float radialFalloff = 0;
     float angularFalloff = 0;
     float angle = 0;
     float lightMultiplier = 0;
     float midAngle = 0;
     float maxAng = 0;
     float minAng = 0;
     float distFromCenter = 0;
     float volLightMultiplier = 0;
     Color trans{ 0,0,0,0 };

     for (x = 0; x < inputBuffer->size.x; ++x)
     {
         for (y = 0; y < inputBuffer->size.y; ++y)
         {
             for (int i = 0; i < numLights + 1; ++i)
             {
                 if (lightSource[i].intensity != 0 || i > numLights)
                 {
                     radialFalloff = 0;
                     angularFalloff = 0;
                     angle = 0;
                     lightMultiplier = 0;
                     midAngle = 0;
                     maxAng = 0;
                     minAng = 0;
                     distFromCenter = 0;
                     volLightMultiplier = 0;
                     angularFalloff = 0;
                     radialFalloff = 0;

                     distFromCenter = distance(lightSource[i].position.x, lightSource[i].position.y, (float)x, (float)y); //find distance from the center of the light   

                     if (lightSource[i].angularWeight != 0)
                     {
                         if (lightSource[i].maxAngle + lightSource[i].angle > 360)
                         {
                             lightSource[i].angle -= 360;
                         }
                         else if (lightSource[i].maxAngle + lightSource[i].angle < 0)
                         {
                             lightSource[i].angle += 360;
                         }

                         angle = atan2(x - lightSource[i].position.x, y - lightSource[i].position.y) * 57.295779f; //Find angle from point to center relative to x axis, magic number is 180 / pi
                         maxAng = lightSource[i].maxAngle + lightSource[i].angle; //rotate relitive angles to be world space angles
                         minAng = lightSource[i].minAngle + lightSource[i].angle;
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
                             angularFalloff = -1 * (((abs(angle - midAngle) - lightSource[i].maxAngle)) / (lightSource[i].maxAngle - lightSource[i].minAngle));
                             angularFalloff = clamp(lightSource[i].angularWeight * angularFalloff, 0, 1);
                         }
                     }
                     else
                     {
                         angularFalloff = 1;
                     }
                     if (lightSource[i].radialWeight != 0)
                     {
                         //calculate radialfalloff
                         radialFalloff = 1 / (1 + (lightSource[i].radialMult1 * distFromCenter) + (lightSource[i].radialMult2 * (distFromCenter * distFromCenter)));
                         radialFalloff = clamp(lightSource[i].radialWeight * radialFalloff, 0, 1);
                     }
                     else
                     {
                         radialFalloff = 1;
                     }


                     //Calculate objects light
                     if (i == 0)
                     {
                         //clear old buffer if first loop through
                         outputBuffer->buffer[x][y] = trans;
                     }

                     inputBuffer->MergeLayersIndvPixel(backgroundLayer, objectLayer, x, y);
                     lightMultiplier = lightSource[i].intensity * radialFalloff * angularFalloff;
                     if (lightSource[i].isBaked == 0 && lightSource[i].isStatic == 1)
                     {
                         bakedLightsBuffer->buffer[x][y] = (lightSource[i].color * lightMultiplier);
                         lightSource[i].isBaked = 1;
                     }
                     outputBuffer->buffer[x][y] += (inputBuffer->buffer[x][y] * ((lightSource[i].color * lightMultiplier) / 255));

                     lightMultiplier *= lightSource[i].volumetricIntensity;
                     outputBuffer->buffer[x][y] += (lightSource[i].color * lightMultiplier);
                 }
                 else
                 {
                     outputBuffer->buffer[x][y] += (inputBuffer->buffer[x][y] * (bakedLightsBuffer->buffer[x][y] / 255));
                     outputBuffer->buffer[x][y] += (lightSource[i].color * (bakedLightsBuffer->buffer[x][y] * lightSource[i].volumetricIntensity));
                 }
             }
         }
     }
     return;
};

 void Renderer::BakeLights()
 {
     int x = 0;
     int y = 0;
     int i = 0;
     float radialFalloff = 0;
     float angularFalloff = 0;
     float angle = 0;
     float lightMultiplier = 0;
     float midAngle = 0;
     float maxAng = 0;
     float minAng = 0;
     float distFromCenter = 0;
     float volLightMultiplier = 0;
     Color trans{ 0,0,0,0 };
   
     for (x = 0; x < inputBuffer->size.x; ++x)
     {
         for (y = 0; y < inputBuffer->size.y; ++y)
         {
             for (i = 0; i < numStaticLights; ++i)
             {
                 radialFalloff = 0;
                 angularFalloff = 0;
                 angle = 0;
                 lightMultiplier = 0;
                 midAngle = 0;
                 maxAng = 0;
                 minAng = 0;
                 distFromCenter = 0;
                 volLightMultiplier = 0;
                 angularFalloff = 0;
                 radialFalloff = 0;

                 distFromCenter = distance(staticLightSource[i].position.x, staticLightSource[i].position.y, (float)x, (float)y); //find distance from the center of the light   

                 if (staticLightSource[i].angularWeight != 0)
                 {
                     if (staticLightSource[i].maxAngle + staticLightSource[i].angle > 360)
                     {
                         staticLightSource[i].angle -= 360;
                     }
                     else if (staticLightSource[i].maxAngle + staticLightSource[i].angle < 0)
                     {
                         staticLightSource[i].angle += 360;
                     }

                     angle = atan2(x - staticLightSource[i].position.x, y - staticLightSource[i].position.y) * 57.295779f; //Find angle from point to center relative to x axis, magic number is 180 / pi
                     maxAng = staticLightSource[i].maxAngle + staticLightSource[i].angle; //rotate relitive angles to be world space angles
                     minAng = staticLightSource[i].minAngle + staticLightSource[i].angle;
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
                         angularFalloff = -1 * (((abs(angle - midAngle) - staticLightSource[i].maxAngle)) / (staticLightSource[i].maxAngle - staticLightSource[i].minAngle));
                         angularFalloff = clamp(staticLightSource[i].angularWeight * angularFalloff, 0, 1);
                     }
                 }
                 else
                 {
                     angularFalloff = 1;
                 }
                 if (staticLightSource[i].radialWeight != 0)
                 {
                     //calculate radialfalloff
                     radialFalloff = 1 / (1 + (staticLightSource[i].radialMult1 * distFromCenter) + (staticLightSource[i].radialMult2 * (distFromCenter * distFromCenter)));
                     radialFalloff = clamp(staticLightSource[i].radialWeight * radialFalloff, 0, 1);
                 }
                 else
                 {
                     radialFalloff = 1;
                 }

                 if (i == 0)
                 {
                     //clear old buffer if first loop through
                     bakedLightsBuffer->buffer[x][y] = trans;
                 }
                 lightMultiplier = staticLightSource[i].intensity * radialFalloff * angularFalloff;
                 bakedLightsBuffer->buffer[x][y] += (staticLightSource[i].color * lightMultiplier);
                 if (bakedLightsBuffer->buffer[x][y].r > 150)
                 {
                     int h = 0;
                 }
             }
         }
     }
 }

 Renderer::Renderer()
 {
     outputBuffer = new ImageBuffer;
     inputBuffer = new ImageBuffer;
     objectLayer = new ImageBuffer;
     backgroundLayer = new ImageBuffer;
     bakedLightsBuffer = new ImageBuffer;
     outputBuffer->screenScale = screenScale;
 }

 void Renderer::Update()
 {
     RenderLightingPass();
     SDL_RenderSetScale(renderer, outputBuffer->screenScale, outputBuffer->screenScale);
     //debug count something code
     /*
     for(int i = 0; i < thing to count * 2; i++)
     {
        if(i % 2 == 0)
        {
            outputBuffer->buffer[i + 1][0];
        }
     }
     */
     for (int x = 0; x < outputBuffer->BufferSizeX; ++x)
     {
         for (int y = 0; y < outputBuffer->BufferSizeY; ++y)
         {
             SDL_SetRenderDrawColor(renderer, outputBuffer->buffer[x][y].r, outputBuffer->buffer[x][y].g, outputBuffer->buffer[x][y].b, 255);
             SDL_RenderDrawPoint(renderer, x, y);
         }
     }

     SDL_RenderPresent(renderer);
 }

 void Renderer::AddObject(ImageBuffer* sprite)
 {
     objectLayer->ClearImageBuffer();
     if (numObjects + 1 > !MAX_OBJECTS)
     {
         objects[numObjects] = sprite;
         numObjects += 1;
     }
     for (int l = 0; l < 3; ++l)
     {
         for (int i = 0; i < numObjects; ++i)
         {
             if (objects[i]->layer == l)
             {
                 objectLayer->AddSprite(objects[i]);
             }
         }
     }
 }

 void Renderer::UpdateObjects()
 {
     objectLayer->ClearImageBuffer();
     for (int l = 0; l < 3; ++l)
     {
         for (int i = 0; i < numObjects; ++i)
         {
             if (objects[i]->layer == l)
             {
                 objectLayer->AddSprite(objects[i]);
             }
         }
     }
 }

 ImageBuffer* Renderer::GetObjectByName(std::string name_)
 {
     for (int i = 0; i < numObjects; ++i)
     {
         if (objects[i]->name == name_)
         {
             return objects[i];
         }
     }
     return NULL;
 }

 void Renderer::AddLight(Light light)
 {
     if (numLights + numStaticLights + 1 < MAX_LIGHT_SOURCES)
     {
         if (light.isStatic == 0)
         {
             lightSource[numLights] = light;
             numLights += 1;
         }
         else
         {
             staticLightSource[numStaticLights] = light;
             numStaticLights += 1;
             BakeLights();
         }
     }
 }
