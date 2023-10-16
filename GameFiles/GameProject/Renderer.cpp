//------------------------------------------------------------------------------
//
// File Name:	Renderer.cpp
// Author(s):	Tyler Dean, Thomas Stephenson
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
#include "ScopeTimer.h"

#include <SDL/SDL.h>
#include <glad/glad.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <string>
#include <assert.h>

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
    
    backgroundLayer->Blit(inputBuffer);
    objectLayer->Blit(inputBuffer);
    tileMapLayer->Blit(inputBuffer);
    
    outputBuffer->ClearImageBuffer();
    
    for (x = 0; x < inputBuffer->size.x; ++x)
    {
        for (y = 0; y < inputBuffer->size.y; ++y)
        {
            for (i = 0; i < numLights; ++i)
            {
                lightMultiplier = FindPixelLuminosity(x, y, i, lightSource);
                if (lightMultiplier != 0)
                {
                    rScale = (lightSource[i].color.GetRed() * lightMultiplier) / 255;
                    gScale = (lightSource[i].color.GetGreen() * lightMultiplier) / 255;
                    bScale = (lightSource[i].color.GetBlue() * lightMultiplier) / 255;
                        
                    outputBuffer->SampleColor(x, y) += inputBuffer->SampleColor(x, y).ScaleIndividual(rScale, gScale, bScale);
                        
                    lightMultiplier *= lightSource[i].volumetricIntensity;
                    outputBuffer->SampleColor(x, y) += (lightSource[i].color * lightMultiplier);
                }
            }
        }
    }
}

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
                    bakedLightsBuffer->SampleColor(x, y) = trans;
                    bakedIntensity[x][y] = 0;
                }

                lightMultiplier = FindPixelLuminosity(x, y, i, staticLightSource);
                if (lightMultiplier != 0)
                {
                    bakedIntensity[x][y] += lightMultiplier;
                    bakedLightsBuffer->SampleColor(x, y) += staticLightSource[i].color;
                    bakedLightsBuffer->SampleColor(x, y).SetAlpha(255);
                }
            }
        }
    }
 }

float Renderer::FindPixelLuminosity(float x, float y, int i, Light lightSource_[MAX_LIGHT_SOURCES])
{
    Light *LightSource = lightSource_ + i;
    float Result = 0.0f;
    
    switch(LightSource->Type)
    {
        case LightSourceType_Point:
        {
            Result = 1.0f / Vector2::Length(Vector2(x, y) - LightSource->position);
        } break;

        case LightSourceType_Directional:
        {
            // @TODO: Check this against actual real-world spotlights.
            float DegreesToRadians = 0.01745329251994329577f;
            float AngleRadians = DegreesToRadians * -LightSource->angle;
            float MinAngleRadians = DegreesToRadians * -LightSource->minAngle;
            float MaxAngleRadians = DegreesToRadians * -LightSource->maxAngle;

            // @TODO: Precompute this?
            Vector2 FrustumLeftEdge = Vector2(-sinf(MinAngleRadians), cosf(MinAngleRadians));
            Vector2 FrustumRightEdge = Vector2(-sinf(MaxAngleRadians), cosf(MaxAngleRadians));
            float dRadius = Vector2::Length(FrustumRightEdge - FrustumLeftEdge);
            
            Vector2 EmissionDirection = Vector2(-sinf(AngleRadians), cosf(AngleRadians));
            Vector2 EmissionTangent = Vector2(-EmissionDirection.y, EmissionDirection.x);
            Vector2 ToPixel = Vector2(x, y) - LightSource->position;

            float EPSILON = 0.0;
            float Dot = Vector2::DotProduct(ToPixel, EmissionDirection);
            if(Dot > EPSILON)
            {
                float Distance = Vector2::Length(ToPixel);
                float Radius = sqrt(Dot) * dRadius;
                float EmitterDistance = 1.0f / (Distance*Distance);
                float FrustumDistance = (Radius - sqrtf(fabsf(Vector2::DotProduct(EmissionTangent, ToPixel))));
                if(FrustumDistance < 0.0f)
                {
                    FrustumDistance = 0.0f;
                }
                if(FrustumDistance > 1.0f)
                {
                    FrustumDistance = 1.0f;
                }
                
                Result = EmitterDistance * FrustumDistance;
            }
        } break;

		default:
		{
			assert(!"Encountered a light source of an unknown type.");
		} break;
    }

    Result *= LightSource->intensity;
    
    return(Result);
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
                testBackgroundTile->SampleColor(x, y) = white;
            }
            else
                testBackgroundTile->SampleColor(x, y) = grey;
        }
    }
    ImageBuffer* testWallTile = new ImageBuffer(15, 15);
    for (int x = 0; x < testWallTile->BufferSizeX; ++x)
    {
        for (int y = 0; y < testWallTile->BufferSizeY; ++y)
        {
            testWallTile->SampleColor(x, y) = black;
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
    PreviousFrameBeginTime = startTime;
}

Renderer::~Renderer(void)
{
    delete outputBuffer;
    delete inputBuffer;
    delete objectLayer;
    delete backgroundLayer;
    delete tileMapLayer;
    delete bakedLightsBuffer;

    glDeleteTextures(1, &OutputBufferTexture);
}

void Renderer::Update()
{
    Uint32 currentTime = SDL_GetTicks();
    ScopeTimer TestScopeTimer("Renderer::Update");

    if(numStaticLights && !LightsBaked)
    {
		//BakeLights();		
		LightsBaked = true;
    }
     
    RenderLightingPass();

    //debug count something code
    ++frameCount;
    if (currentTime - startTime >= 1000) {
        shut_up = frameCount;
        frameCount = 0;
        startTime = currentTime;
    }

    float FrameRate = 1000.0f / (float)(currentTime - PreviousFrameBeginTime);
    std::string fpsString = std::to_string(FrameRate);
    SDL_SetWindowTitle(window, fpsString.c_str());

    for (int i = 0; i < numLights * 2; i++)
    {
        if (i % 2 == 0)
        {
            outputBuffer->buffer[i + 1 + (3 * outputBuffer->BufferSizeX)] = { 0,0,255,255 };
        }
    }

    if(!OutputBufferTexture)
    {
        glGenTextures(1, &OutputBufferTexture);
    }

    glClearColor(1.0f, 0.0f, 1.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    
    glEnable(GL_TEXTURE_2D);
    
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, OutputBufferTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8,
                 outputBuffer->BufferSizeX, outputBuffer->BufferSizeY,
                 0,
                 GL_RGBA, GL_UNSIGNED_BYTE,
                 (void *)outputBuffer->buffer);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    
    glBegin(GL_QUADS);
    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

    glTexCoord2f(0.0f, 0.0f);
    glVertex2f(-1.0f, 1.0f);

    glTexCoord2f(1.0f, 0.0f);
    glVertex2f(1.0f, 1.0f);

    glTexCoord2f(1.0f, 1.0f);
    glVertex2f(1.0f, -1.0f);

    glTexCoord2f(0.0f, 1.0f);
    glVertex2f(-1.0f, -1.0f);

    glEnd();
    
    SDL_GL_SwapWindow(window);
    
    objectLayer->ClearImageBuffer();
    PreviousFrameBeginTime = currentTime;
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
    //objectLayer->ClearImageBuffer();
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
     //objectLayer->ClearImageBuffer();
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
        }
    }
}
