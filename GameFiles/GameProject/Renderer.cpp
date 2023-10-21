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

Vector2 Renderer::GetCameraPosition(void)
{
	return CameraP;
}

void Renderer::SetCameraPosition(Vector2 NewCameraP)
{
	CameraP = NewCameraP;
}

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
    
	int CameraOffsetX = (int)CameraP.x;
	int CameraOffsetY = (int)CameraP.y;

	inputBuffer->ClearImageBuffer();
	outputBuffer->ClearImageBuffer();
	
	backgroundLayer->Blit(inputBuffer, -CameraOffsetX, -CameraOffsetY);
	tileMapLayer->Blit(inputBuffer, -CameraOffsetX, -CameraOffsetY);
	objectLayer->Blit(inputBuffer);

#if 1
	float const OneOver255 = 1.0f / 255.0f;
	
	for (x = 0; x < inputBuffer->size.x; ++x)
    {
        for (y = 0; y < inputBuffer->size.y; ++y)
        {
			float IntensityR = 0.0f;
			float IntensityG = 0.0f;
			float IntensityB = 0.0f;

            for (i = 0; i < numLights; ++i)
            {
				Light *LightSource = lightSource + i;
                float lightMultiplier = FindPixelLuminosity(x, y, LightSource);

				//if (lightMultiplier != 0)
                {
                    float R_F32 = ((float)LightSource->color.GetRed())    * OneOver255;
                    float G_F32 = ((float)LightSource->color.GetGreen())  * OneOver255;
                    float B_F32 = ((float)LightSource->color.GetBlue())   * OneOver255;
                        
                    lightMultiplier *= LightSource->volumetricIntensity;
                    
					IntensityR += lightMultiplier * R_F32;
					IntensityG += lightMultiplier * G_F32;
					IntensityB += lightMultiplier * B_F32;
				}
            }
		
			Color &DestPixel = outputBuffer->SampleColor(x, y);
			DestPixel = inputBuffer->SampleColor(x, y).ScaleIndividual(IntensityR, IntensityG, IntensityB);
		}
    }
#else
	inputBuffer->Blit(outputBuffer);
#endif
}

float Renderer::FindPixelLuminosity(float x, float y, Light *LightSource)
{
    Vector2 LightP = LightSource->position - CameraP;
    float Result = 0.0f;
    
    switch(LightSource->Type)
    {
        case LightSourceType_Point:
        {
			Vector2 D = Vector2(x, y) - LightP;
            float DistanceSq = Vector2::DotProduct(D, D);
            Result = 1.0f / (DistanceSq);
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
            Vector2 ToPixel = Vector2(x, y) - LightP;

            float Dot = Vector2::DotProduct(ToPixel, EmissionDirection);
            if(Dot > 0.0f)
            {
                float DistanceSq = Vector2::DotProduct(ToPixel, ToPixel);
                float Radius = Dot * dRadius;
                float EmitterDistance = 1.0f / (DistanceSq);
                float FrustumDistance = (Radius - (fabsf(Vector2::DotProduct(EmissionTangent, ToPixel))));
                if(FrustumDistance < 0.0f)
                {
                    FrustumDistance = 0.0f;
                }
                if(FrustumDistance > Radius)
                {
                    FrustumDistance = Radius;
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
	
	//float const LIGHTING_STEP_SIZE = 0.25f;
	//Result = LIGHTING_STEP_SIZE * floorf(Result / LIGHTING_STEP_SIZE);

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

    RenderLightingPass();

    float AverageFrameLength = 0.0f;
	for(uint32_t FrameIndex = 1; FrameIndex < _countof(PreviousFrameLengths); ++FrameIndex)
	{
        AverageFrameLength += PreviousFrameLengths[FrameIndex - 1];
        PreviousFrameLengths[FrameIndex] = PreviousFrameLengths[FrameIndex - 1];
	}

    float dtThisFrame = (float)(currentTime - PreviousFrameBeginTime) / 1000.0f;
    PreviousFrameLengths[0] = dtThisFrame;
    AverageFrameLength += dtThisFrame;
    AverageFrameLength /= (float)_countof(PreviousFrameLengths);
    
	float AverageFrameRate = 1.0f / AverageFrameLength;
	char WindowTextBuffer[128];
	sprintf_s(WindowTextBuffer, sizeof(WindowTextBuffer), "FPS: %.2f", AverageFrameRate);
    SDL_SetWindowTitle(window, WindowTextBuffer);

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

	// @TODO: Maybe rework image buffers to be alligned to GPU memory layouts
	assert(sizeof(Color) == sizeof(uint32_t));

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
    if ((numObjects + 1) < MAX_OBJECTS)
    {
        objects[numObjects] = sprite;
        numObjects += 1;
    }
}

void Renderer::UpdateObjects()
{
    objectLayer->ClearImageBuffer();

    for(int i = 0; i < numObjects; ++i)
    {
        for(int l = 0; l < 3; ++l)
        {
            if (objects[i]->layer == l)
            {
                objectLayer->AddSprite(objects[i], CameraP);
                break;
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
		lightSource[numLights++] = light;
	}
}
