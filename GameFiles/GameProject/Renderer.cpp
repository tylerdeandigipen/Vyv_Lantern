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

#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <string>
//BaseSystem::BaseSystem(const char* _name) : name(_name) {}


 void Renderer::RenderLightingPass()
{
     int x = 0;
     int y = 0;
     int i = 0;
     float lightMultiplier = 0;
     float rScale = 0;
     float gScale = 0;
     float bScale = 0;
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
                     inputBuffer->MergeLayersIndvPixel(backgroundLayer, objectLayer, tileMapLayer, x, y);
                     lightMultiplier = FindPixelLuminosity(x, y, i, lightSource);  
                     if (lightMultiplier != 0)
                     {
                         rScale = (lightSource[i].color.GetRed() * lightMultiplier) / 255;
                         gScale = (lightSource[i].color.GetGreen() * lightMultiplier) / 255;
                         bScale = (lightSource[i].color.GetBlue() * lightMultiplier) / 255;

                         outputBuffer->buffer[x][y] += inputBuffer->buffer[x][y].ScaleIndividual(rScale, gScale, bScale);

                         lightMultiplier *= lightSource[i].volumetricIntensity;
                         outputBuffer->buffer[x][y] += (lightSource[i].color * lightMultiplier);
                     }
                 }
                 else if(bakedLightsBuffer->buffer[x][y].intensity != 0)
                 {
                     lightMultiplier = bakedLightsBuffer->buffer[x][y].intensity;
                     rScale = (bakedLightsBuffer->buffer[x][y].GetRed() * lightMultiplier) / 255;
                     gScale = (bakedLightsBuffer->buffer[x][y].GetGreen() * lightMultiplier) / 255;
                     bScale = (bakedLightsBuffer->buffer[x][y].GetBlue() * lightMultiplier) / 255;
                     outputBuffer->buffer[x][y] += inputBuffer->buffer[x][y].ScaleIndividual(rScale, gScale, bScale);
                     
                     lightMultiplier *= bakedVolumetricIntensity;
                     outputBuffer->buffer[x][y] += (bakedLightsBuffer->buffer[x][y] * lightMultiplier);
                 }
             }
         }
     }
     return;
};

 void Renderer::BakeLights()
 {
     Color trans{ 0,0,0,0 };
     float lightMultiplier;
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
                     bakedLightsBuffer->buffer[x][y].intensity = 0;
                 }

                 lightMultiplier = FindPixelLuminosity(x, y, i, staticLightSource);
                 if (lightMultiplier != 0)
                 {
                    bakedLightsBuffer->buffer[x][y].intensity += lightMultiplier;
                    bakedLightsBuffer->buffer[x][y] += staticLightSource[i].color; 
                    bakedLightsBuffer->buffer[x][y].SetAlpha(255);
                 }
             }
             bakedLightsBuffer->buffer[x][y].intensity /= numStaticLights;
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
                 if (angularFalloff < 0)
                 {
                     return 0;
                 }
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
             //maybe change to dist squared later?
             distFromCenter = distance(lightSource_[i].position.x, lightSource_[i].position.y, (float)x, (float)y); //find distance from the center of the light   
             radialFalloff = 1 / (1 + (lightSource_[i].radialMult1 * distFromCenter) + (lightSource_[i].radialMult2 * (distFromCenter * distFromCenter)));
             if (radialFalloff < 0)
             {
                 return 0;
             }
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

 void Renderer::MakeTileMap(int tileMapArray[16][9])
 {
     Color trans{ 0,0,0,0 };

     //make test tiles to make tilemap with (temp test tiles)
     Color white(255, 255, 255, 255);
     Color black(1, 1, 1, 255);
     Color grey = { 150, 150, 150, 255 };
     Color blue(50, 100, 255, 255);
     ImageBuffer* testBackgroundTile = new ImageBuffer(15, 15);
     for (int x = 0; x < testBackgroundTile->BufferSizeX; ++x)
     {
         for (int y = 0; y < testBackgroundTile->BufferSizeY; ++y)
         {
             if (x % 3 != 0 && y % 3 != 0)
             {
                 testBackgroundTile->buffer[x][y] = white;
             }
             else
                 testBackgroundTile->buffer[x][y] = grey;
         }
     }
     ImageBuffer* testWallTile = new ImageBuffer(15, 15);
     for (int x = 0; x < testWallTile->BufferSizeX; ++x)
     {
         for (int y = 0; y < testWallTile->BufferSizeY; ++y)
         {
                 testWallTile->buffer[x][y] = black;
         }
     }
     //end of test tiles
     tileMapLayer->ClearImageBuffer();
     backgroundLayer->ClearImageBuffer();

     for (int x = 0; x < 16; ++x)
     {
         for (int y = 0; y < 9; ++y)
         {

             switch (tileMapArray[x][y])
             {
                 case 0:
                     testBackgroundTile->position = { (float)(x * 15), (float)(y * 15) };
                     backgroundLayer->AddSprite(testBackgroundTile);
                     break;
                 case 1:
                     testWallTile->position = { (float)(x * 15), (float)(y * 15) };
                     tileMapLayer->AddSprite(testWallTile);
                     break;
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
     tileMapLayer = new ImageBuffer;
     outputBuffer->screenScale = screenScale;
     startTime = SDL_GetTicks();
 }

 int needToSetSreenSize = 0;

 void Renderer::Update()
 {
     Uint32 currentTime = SDL_GetTicks();

     if (needToSetSreenSize == 0)
     {
         SDL_RenderSetScale(renderer, outputBuffer->screenScale, outputBuffer->screenScale);
         needToSetSreenSize = 1;
     }

     RenderLightingPass();
     //debug count something code
     ++frameCount;
     if (currentTime - startTime >= 1000) {
         shut_up = frameCount;
         frameCount = 0;
         startTime = currentTime;
     }
     std::string fpsString = std::to_string(shut_up);
     SDL_SetWindowTitle(window, fpsString.c_str());

     for (int i = 0; i < numLights * 2; i++)
     {
         if (i % 2 == 0)
         {
             outputBuffer->buffer[i + 1][3] = { 0,0,255,255 };
         }
     }
     for (int x = 0; x < outputBuffer->BufferSizeX; ++x)
     {
         for (int y = 0; y < outputBuffer->BufferSizeY; ++y)
         {
             SDL_SetRenderDrawColor(renderer, outputBuffer->buffer[x][y].GetRed(), outputBuffer->buffer[x][y].GetGreen(), outputBuffer->buffer[x][y].GetBlue(), 255);
             SDL_RenderDrawPoint(renderer, x, y);
         }
     }

     SDL_RenderPresent(renderer);
 }

 int Renderer::returnObjCnt()
 {
     int countObjects = 0;
     for (int i = 0; i < MAX_OBJECTS; ++i) {
         if (objects[i] != nullptr) {
             countObjects++;
         }
     }
     return countObjects;
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
