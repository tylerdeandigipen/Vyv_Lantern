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
     int i = 0;
     float lightMultiplier = 0;

     Color trans{ 0,0,0,0 };

     for (x = 0; x < inputBuffer->size.x; ++x)
     {
         for (y = 0; y < inputBuffer->size.y; ++y)
         {
             outputBuffer->buffer[x][y] = trans;
             for (i = 0; i < numLights + 1; ++i)
             {
                 if (i < numLights)
                 {
                     inputBuffer->MergeLayersIndvPixel(backgroundLayer, objectLayer, x, y);
                   
                     lightMultiplier = FindPixelLuminosity(x, y, i, lightSource);  
                     if (lightMultiplier != 0)
                     {
                         outputBuffer->buffer[x][y] += (inputBuffer->buffer[x][y] * ((lightSource[i].color * lightMultiplier) / 255));

                         lightMultiplier *= lightSource[i].volumetricIntensity;
                         outputBuffer->buffer[x][y] += (lightSource[i].color * lightMultiplier);
                     }
                 }
                 else if(bakedLightsBuffer->buffer[x][y].a != 0)
                 {
                     outputBuffer->buffer[x][y] += (inputBuffer->buffer[x][y] * (bakedLightsBuffer->buffer[x][y] / 255));
                     outputBuffer->buffer[x][y] += (bakedLightsBuffer->buffer[x][y] * bakedVolumetricIntensity);
                 }
             }
         }
     }
     return;
};

 void Renderer::BakeLights()
 {
     Color trans{ 0,0,0,0 };
     float lumiosity;
     int x = 0;
     int y = 0;
     int i = 0;
     for (x = 0; x < inputBuffer->size.x; ++x)
     {
         for (y = 0; y < inputBuffer->size.y; ++y)
         {
             for (int i = 0; i < numStaticLights; ++i)
             {
                 if (i == 0)
                 {
                     bakedLightsBuffer->buffer[x][y] = trans;
                 }

                 lumiosity = FindPixelLuminosity(x, y, i, staticLightSource);
                 bakedLightsBuffer->buffer[x][y] += (staticLightSource[i].color * lumiosity);
             }
         }
     }
 }

 float Renderer::FindPixelLuminosity(float x, float y, int i, Light lightSource_[MAX_LIGHT_SOURCES])
 {
     if (lightSource_[i].intensity != 0)
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
         if (lightSource_[i].angularWeight != 0)
         {
             if (lightSource_[i].maxAngle + lightSource_[i].angle > 360)
             {
                 lightSource_[i].angle -= 360;
             }
             else if (lightSource_[i].maxAngle + lightSource_[i].angle < 0)
             {
                 lightSource_[i].angle += 360;
             }

             angle = atan2(x - lightSource_[i].position.x, y - lightSource_[i].position.y) * 57.295779f; //Find angle from point to center relative to x axis, magic number is 180 / pi
             maxAng = lightSource_[i].maxAngle + lightSource_[i].angle; //rotate relitive angles to be world space angles
             minAng = lightSource_[i].minAngle + lightSource_[i].angle;
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
                 angularFalloff = -1 * (((abs(angle - midAngle) - lightSource_[i].maxAngle)) / (lightSource_[i].maxAngle - lightSource_[i].minAngle));
                 angularFalloff = clamp(lightSource_[i].angularWeight * angularFalloff, 0, 1);
             }
         }
         else
         {
             angularFalloff = 1;
         }
         if (lightSource_[i].radialWeight != 0)
         {
             //calculate radialfalloff
             distFromCenter = distance(lightSource_[i].position.x, lightSource_[i].position.y, (float)x, (float)y); //find distance from the center of the light   
             radialFalloff = 1 / (1 + (lightSource_[i].radialMult1 * distFromCenter) + (lightSource_[i].radialMult2 * (distFromCenter * distFromCenter)));
             radialFalloff = clamp(lightSource_[i].radialWeight * radialFalloff, 0, 1);
         }
         else
         {
             radialFalloff = 1;
         }
         lightMultiplier = lightSource_[i].intensity * radialFalloff * angularFalloff;
         return lightMultiplier;
     }
     return 0;     
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
