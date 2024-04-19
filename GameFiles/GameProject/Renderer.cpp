//------------------------------------------------------------------------------
//
// File Name:	Renderer.cpp
// Author(s):	Tyler Dean, Thomas Stephenson, Louis Wang
// Purpose:		The Renderer implementation
//
// Copyright � 2023 DigiPen (USA) Corporation.
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
#include "Engine.h"
#include "Time_TM.h"
#include "SceneSystem.h"
#include "Inputs.h"
#include "DebugNew.h"
#include "imgui_impl_opengl3.h"
#include "FrameRate.h"
#include "SceneSystem.h"
#include "MenuScene.h"

#include <windows.h>
#include <SDL/SDL.h>
#include <glad/glad.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <string>
#include <assert.h>
#include <omp.h>

// Emitter laser checks
#include "Entity.h"
#include "EntityContainer.h"
#include "Emitter.h"

#include "PauseMenu.h"
#include <thread>

#define OneOver255 (1.0f / 255.0f)

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

std::unique_ptr<Renderer> Renderer::instance = nullptr;

//split into Update and Render functions
void Renderer::Update(float dt)
{
	if (Engine::GetInstance()->Paused() == true)
	{
		PauseMenu::GetInstance()->isPauseMenuOpen = true;
		if (pauseMenuIndex == -1)
		{
			pauseMenuIndex = AddMenuPage("./Assets/PPM/Pause_Temp.ppm");
		}
		if (exitConfirmIndex == -1)
		{
			exitConfirmIndex = AddMenuPage("./Assets/PPM/Pause_Menu_Confirm_Quit.ppm");
		}
		if (settingsMenuIndex == -1)
		{
			settingsMenuIndex = AddMenuPage("./Assets/PPM/Pause_Menu_Settings.ppm");
		}

		LoadMenuPage(pauseMenuIndex);
	}
	else if (SceneSystem::GetInstance()->mainMenuOpen == true)
	{
		if (creditsMainMenuIndex == -1)
		{
			creditsMainMenuIndex = AddMenuPage("./Assets/PPM/Credits_Menu.ppm");
		}
		if (confirmQuitMainindex == -1)
		{
			confirmQuitMainindex = AddMenuPage("./Assets/PPM/Main_Menu_Confirm_Quit.ppm");
		}
		if (mainMenuOptionsIndex == -1)
		{
			mainMenuOptionsIndex = AddMenuPage("./Assets/PPM/Main_Menu_Settings.ppm");
		}
		if (mainMenuIndex == -1)
		{
			mainMenuIndex = AddMenuPage("./Assets/PPM/Main_Menu.ppm");
		}
		if (mainMenuHelpIndex == -1)
		{
			mainMenuHelpIndex = AddMenuPage("./Assets/PPM/How_To_Play_Menu.ppm");
		}
	}
	else
	{
		LoadMenuPage(-1);
	}

	Uint32 currentTime = SDL_GetTicks();
	ScopeTimer TestScopeTimer("Renderer::Update");

	//make higher if preformance is low, make lower if too much cpu usage
	omp_set_num_threads(maxThreadsAllowed);

	//fixes janky cam movements
	CameraP.x = std::truncf(nextCamPos.x);
	CameraP.y = std::truncf(nextCamPos.y);

	if (decalsAreDirty == true)
	{
		AddDecalsToTilemap();
	}

	UpdateObjects(dt);

	//prep buffers for rendering
	int CameraOffsetX = (int)CameraP.x;
	int CameraOffsetY = (int)CameraP.y;

	outputBuffer->ClearImageBuffer();
	lightBuffer->ClearImageBuffer();
	laserBuffer->ClearImageBuffer();

	backgroundLayer->Blit(outputBuffer, -CameraOffsetX, -CameraOffsetY);
	objectLayer->Blit(outputBuffer, -CameraOffsetX, -CameraOffsetY);
	foregroundLayer->Blit(outputBuffer, -CameraOffsetX, -CameraOffsetY);
	particleManager->UpdateParticles();
	RenderParticles();

	if (isFullbright != true)
	{
		RenderLightingPass();
		laserHandler.UpdateLasers();
		RenderLasers();
		BlurLights(-1, 2);
		outputBuffer->DitherBuffer(outputBuffer, renderOnlyLights, isFullbright, minBrightness, lightR, lightG, lightB);
	}
	RenderToOutbuffer();

	if (DebugBuffer != NULL)
	{
		DebugBuffer->Blit(outputBuffer, -(int)GetCameraPosition().x, -(int)GetCameraPosition().y);
		DebugBuffer->ClearImageBuffer();
	}
	float AverageFrameRate = FrameRate::CalculateAverageFrameRate(PreviousFrameLengths, _countof(PreviousFrameLengths), (float)currentTime, (float)PreviousFrameBeginTime);

	FrameRate::UpdateWindowTitle(window, AverageFrameRate);

	//FrameRate::capFrame(dt);

	if (!OutputBufferTexture)
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
		(GLsizei)outputBuffer->size.x, (GLsizei)outputBuffer->size.y,
		0,
		GL_RGBA, GL_UNSIGNED_BYTE,
		(void*)outputBuffer->buffer);
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

#ifdef _DEBUG
	if (ImGui::GetDrawData())
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
#endif
	SDL_GL_SwapWindow(window);
	objectLayer->ClearImageBuffer();
	PreviousFrameBeginTime = currentTime;
}

void Renderer::RenderToOutbuffer()
{
	const int xSize = (int)outputBuffer->size.x;
	const int ySize = (int)outputBuffer->size.y;
	if (renderWallHitboxes != true)
	{
#pragma omp parallel for collapse(2)
		for (int x = 0; x < xSize; ++x)
		{
			for (int y = 0; y < ySize; ++y)
			{
				Color& DestPixel = outputBuffer->SampleColor(x, y);
				if (saturationPercent != 1)
				{
					DestPixel.ChangeSaturation(saturationPercent);
				}

				if (renderNormalMap != true)
				{
					if (y % 2 == 0 && doScanLines == true)
					{
						DestPixel = DestPixel * scanLineOpacity;
					}
				}
				else if (renderNormalMap == true)
				{
					DestPixel = laserBuffer->SampleColor(x, y);
				}
				if (fadePercent < 1)
				{
					DestPixel = DestPixel.BlendColors(DestPixel, fadeColor, 100 * (1 - fadePercent));
				}
			}
		}
	}

	if (renderWallHitboxes == true)
	{
		RenderWallCollidersToDebugBuffer();
	}
	if (currentMenu != -1)
	{
		RenderMenu();
		RenderMenuSelections();
	}
}

void Renderer::RenderLightingPass()
{
#if 1
	const int xSize = (int)outputBuffer->size.x;
	const int ySize = (int)outputBuffer->size.y;

	float lightMultiplier = 0.0f;

	float IntensityR = 0.0f;
	float IntensityG = 0.0f;
	float IntensityB = 0.0f;
#pragma omp parallel
	{
#pragma omp for collapse(3) nowait private(lightMultiplier, IntensityR, IntensityG, IntensityB)
		for (int x = 0; x < xSize; ++x)
		{
			for (int y = 0; y < ySize; ++y)
			{
				IntensityR = 0.0f;
				IntensityG = 0.0f;
				IntensityB = 0.0f;

				for (int i = 0; i < numLights; ++i)
				{
					if (CalculateIfPixelIsLit(x, y, i) == true || doFog == true)
					{
						Light* LightSource = lightSource + i;
						lightMultiplier = FindPixelLuminosity((float)x, (float)y, LightSource);

						if (lightMultiplier != 0)
						{
							float R_F32 = ((float)LightSource->color.GetRed()) * OneOver255;
							float G_F32 = ((float)LightSource->color.GetGreen()) * OneOver255;
							float B_F32 = ((float)LightSource->color.GetBlue()) * OneOver255;
							lightMultiplier *= LightSource->volumetricIntensity;

							IntensityR += (lightMultiplier * R_F32);
							IntensityG += (lightMultiplier * G_F32);
							IntensityB += (lightMultiplier * B_F32);
						}
					}
				}

				lightR[x][y] = IntensityR;
				lightG[x][y] = IntensityG;
				lightB[x][y] = IntensityB;
			}
		}
	}
#else
	inputBuffer->Blit(outputBuffer);
#endif
}

float Renderer::FindPixelLuminosity(float x, float y, Light* LightSource)
{
	float normalMin = 0.75f;
	Vector2 LightP = LightSource->position - CameraP;
	float Result = 0.0f;

	switch (LightSource->Type)
	{
	case LightSourceType_Point:
	{
		Vector2 D = Vector2(x, y) - LightP;
		float DistanceSq = Vector2::DotProduct(D, D);
		float distance = sqrt(DistanceSq);
		float s = distance / LightSource->radius;
		if (s >= 1.0)
			return 0.0;
		float s2 = s * s;
		Result = LightSource->intensity * (float)pow((1 - s2), 2) / (1 + LightSource->radialFalloff * s);
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
		if (Dot > 0.0f)
		{
			float DistanceSq = Vector2::DotProduct(ToPixel, ToPixel);
			float Radius = Dot * dRadius;
			float distance = sqrt(DistanceSq);
			float s = distance / LightSource->radius;
			if (s >= 1.0)
				return 0.0;
			float s2 = s * s;
			float EmitterDistance = LightSource->intensity * (float)pow((1 - s2), 2) / (1 + LightSource->radialFalloff * s);
			float FrustumDistance = (Radius - (fabsf(Vector2::DotProduct(EmissionTangent, ToPixel))));
			if (FrustumDistance < 0.0f)
			{
				FrustumDistance = 0.0f;
			}
			if (FrustumDistance > Radius)
			{
				FrustumDistance = Radius;
			}

			Result = EmitterDistance * (FrustumDistance * LightSource->frustumWeight);
		}
	} break;

	default:
	{
		assert(!"Encountered a light source of an unknown type.");
	} break;
	}

	//Normal map calculations
	if (Result > 0 && doFog != true)
	{
		if ((int)x + (int)CameraP.x <= normalBuffer->BufferSizeX && (int)x + (int)CameraP.x >= 0 && (int)y + (int)CameraP.y <= normalBuffer->BufferSizeY && (int)y + (int)CameraP.y >= 0)
		{
			float normalR = (float)normalBuffer->SampleColor((int)x + (int)CameraP.x, (int)y + (int)CameraP.y).r;
			float normalG = (float)normalBuffer->SampleColor((int)x + (int)CameraP.x, (int)y + (int)CameraP.y).g;

			if (normalR == 0 && normalG == 0)
			{
				return Result;
			}
			Vector2 pos = (Vector2(x, y));
			Vector2 distFromLight = LightP - pos;
			Vector2 distNormalized;
			Vector2 normalDir = Vector2{ normalR / 255.0f, normalG / 255.0f };
			normalDir *= 2;
			normalDir -= Vector2{ 1,1 };
			distNormalized.Normalize(distNormalized, distFromLight);
			float normalFalloff = -Vector2::DotProduct(distNormalized, normalDir);
			normalFalloff += normalMin;
			normalFalloff = clamp(normalFalloff, 0.0f, 1.0f);
			Result = (normalFalloff * normalStrength * Result);
		}
	}

	return Result;
}

bool Renderer::CalculateIfPixelIsLit(int x, int y, int i)
{
	const Vector2 cameraPos = CameraP;
	float diameter = 0;
	Vector2 lightPos;
	bool isLit = true;
	if (lightSource[i].intensity != 0)
	{
		if (x + (int)CameraP.x >= 0 && x + (int)CameraP.x <= shadowCasterBuffer->BufferSizeX && shadowCasterBuffer->BufferSizeY <= y + (int)CameraP.y && y + (int)CameraP.y >= 0)
		{
			return false;
		}
		diameter = lightSource[i].radius * lightSource[i].radius;
		lightPos = lightSource[i].position;
		if (distanceSquared((float)x, (float)y, lightPos.x - cameraPos.x, lightPos.y - cameraPos.y) <= diameter)
		{
			int inOutCount = CheckLineForObject((int)(lightPos.x - cameraPos.x), (int)(lightPos.y - cameraPos.y), x, y);
			if (inOutCount > 0)
			{
				isLit = false;
				if (shadowCasterBuffer->SampleColor(x + (int)CameraP.x, y + (int)CameraP.y).GetAlpha() != 0 && inOutCount == 1)
				{
					isLit = true;
				}
			}
			else
			{
				isLit = true;
			}
		}
		else
		{
			isLit = false;
		}
	}

	return isLit;
}

void Renderer::RenderMenu()
{
	const int xSize = (int)outputBuffer->size.x;
	const int ySize = (int)outputBuffer->size.y;

#pragma omp parallel for collapse(2)
	for (int x = 0; x < xSize; ++x)
	{
		for (int y = 0; y < ySize; ++y)
		{
			Color& DestPixel = outputBuffer->SampleColor(x, y);
			DestPixel = DestPixel / 2;
		}
	}

	PauseMenu::GetInstance()->HandleInput();

	menuBuffer->Blit(outputBuffer);
}

void Renderer::RenderParticles()
{
	if (this == NULL || foregroundLayer == NULL || particleManager == NULL)
	{
		return;
	}
	Vector2 tempPos;
	particleManager->tileMapSize = Vector2{ (float)foregroundLayer->BufferSizeX, (float)foregroundLayer->BufferSizeY };

#pragma omp for private(tempPos)
	for (int i = 0; i < particleManager->totalParticles; i++)
	{
		if (particleManager->particleArray[i] != NULL)
		{
			tempPos = particleManager->particleArray[i]->position;
			tempPos -= CameraP;
			if (tempPos.x >= 0 && tempPos.x < outputBuffer->size.x && tempPos.y >= 0 && tempPos.y < outputBuffer->size.y)
			{
				Color& DestPixel = outputBuffer->SampleColor((int)tempPos.x, (int)tempPos.y);
				if (particleManager->particleArray[i]->isTransparent == false)
				{
					DestPixel = particleManager->particleArray[i]->color;
				}
				else
				{
					float transPercent = (particleManager->particleArray[i]->timeAlive / particleManager->particleArray[i]->lifeTime);
					DestPixel = DestPixel.BlendColors(particleManager->particleArray[i]->color, DestPixel, transPercent * 100);
				}
			}
		}
	}
}

float laserSize = 2.15f;
float laserWeight = 2;
float laserAreaLightRange = 15;
Color laserAreaLightColor{ 150,150,150,255 };
float lightWeight = 1.4f;
float laserThreshold = .52f;
float lightMultiplier = 1.3f;
void Renderer::RenderLasers()
{
	const int xSize = (int)outputBuffer->size.x;
	const int ySize = (int)outputBuffer->size.y;

	float IntensityR = 0.0f;
	float IntensityG = 0.0f;
	float IntensityB = 0.0f;

	//optimize later to only calculate light near / in the laser line zone
	//maybe make so depending on distance it uses two different light func  tions
#pragma omp parallel
	{
#pragma omp for collapse(3) nowait private(IntensityR, IntensityG, IntensityB)
		for (int x = 0; x < xSize; ++x)
		{
			for (int y = 0; y < ySize; ++y)
			{
				for (int i = 0; i < numLasers; i++)
				{
					IntensityR = 0.0f;
					IntensityG = 0.0f;
					IntensityB = 0.0f;

					//Check if the laser is along the x axis
					if ((int)laserPoints1[i].y - (int)laserPoints2[i].y == 0)
					{
						bool doRender = false;

						//check if x is inbetween the two points
						if (x >= laserPoints1[i].x && x <= laserPoints2[i].x && (int)laserPoints1[i].x - (int)laserPoints2[i].x < 0)
						{
							doRender = true;
						}
						else if (x <= laserPoints1[i].x && x >= laserPoints2[i].x && (int)laserPoints1[i].x - (int)laserPoints2[i].x > 0)
						{
							doRender = true;
						}
						if (doRender == true)
						{
							//check distance from laser line
							if (abs(y - laserPoints2[i].y) < laserAreaLightRange)
							{
								int inOutCount = CheckLineForObject(x, (int)laserPoints2[i].y, x, y);
								bool isLit = false;
								if (inOutCount <= 1)
								{
									isLit = true;
								}
								if (isLit == true)
								{
									float scaledDist = abs(y - (int)laserPoints2[i].y) / laserAreaLightRange;
									float scaledDistSquared = scaledDist * scaledDist;

									float falloff = 0.8f * (float)pow((1 - scaledDistSquared), 2) / (1 + .5f * scaledDist);
									IntensityR = (IntensityR + falloff * (laserAreaLightColor.r / 255.0f));
									IntensityG = (IntensityG + falloff * (laserAreaLightColor.g / 255.0f));
									IntensityB = (IntensityB + falloff * (laserAreaLightColor.b / 255.0f));

									lightR[x][y] += IntensityR * lightWeight * lightMultiplier;
									lightG[x][y] += IntensityG * lightWeight * lightMultiplier;
									lightB[x][y] += IntensityB * lightWeight * lightMultiplier;
								}
								if (abs(y - laserPoints2[i].y) < laserSize)
								{
									float scaledDistFromCenter = (laserSize - abs(y - (int)laserPoints2[i].y)) / laserSize;
									float tempIntensityR = scaledDistFromCenter * ((float)laserColor[i].r / 255.0f);
									float tempIntensityG = scaledDistFromCenter * ((float)laserColor[i].g / 255.0f);
									float tempIntensityB = scaledDistFromCenter * ((float)laserColor[i].b / 255.0f);
									if (tempIntensityR >= laserThreshold || tempIntensityG >= laserThreshold || tempIntensityB >= laserThreshold)
									{
										lightR[x][y] = tempIntensityR * laserWeight;
										lightG[x][y] = tempIntensityG * laserWeight;
										lightB[x][y] = tempIntensityB * laserWeight;

										Color& DestPixel = outputBuffer->SampleColor(x, y);
										DestPixel.r = clampInt8((uint8_t)(lightR[x][y] * 255), 0, 254);
										DestPixel.g = clampInt8((uint8_t)(lightG[x][y] * 255), 0, 254);
										DestPixel.b = clampInt8((uint8_t)(lightB[x][y] * 255), 0, 254);
									}
								}
							}
						}
						else if ((x <= laserPoints1[i].x + laserAreaLightRange && x >= laserPoints2[i].x - laserAreaLightRange && (int)laserPoints1[i].x - (int)laserPoints2[i].x > 0) || (x >= laserPoints1[i].x - laserAreaLightRange && x <= laserPoints2[i].x + laserAreaLightRange && (int)laserPoints1[i].x - (int)laserPoints2[i].x < 0))
						{
							if (abs(y - laserPoints2[i].y) < laserAreaLightRange)
							{
								float distSquared = abs(distanceSquared(laserPoints2[i].x, laserPoints2[i].y, (float)x, (float)y));
								if (distSquared < laserAreaLightRange * laserAreaLightRange)
								{
									int inOutCount = CheckLineForObject((int)laserPoints2[i].x, (int)laserPoints2[i].y, x, y);
									bool isLit = false;
									if (inOutCount <= 1)
									{
										isLit = true;
									}
									if (isLit == true)
									{
										float dist = sqrt(distSquared);
										float s = dist / laserAreaLightRange;
										float s2 = s * s;
										float falloff = 0.8f * (float)pow((1 - s2), 2) / (1 + .5f * s);
										IntensityR = (IntensityR + falloff * (laserAreaLightColor.r / 255.0f));
										IntensityG = (IntensityG + falloff * (laserAreaLightColor.g / 255.0f));
										IntensityB = (IntensityB + falloff * (laserAreaLightColor.b / 255.0f));

										lightR[x][y] += IntensityR * lightWeight * lightMultiplier;
										lightG[x][y] += IntensityG * lightWeight * lightMultiplier;
										lightB[x][y] += IntensityB * lightWeight * lightMultiplier;
									}
								}
							}
						}
					} // Check if laser is along the y axis
					else if ((int)laserPoints1[i].x - (int)laserPoints2[i].x == 0)
					{
						bool doRender = false;

						//check if y is inbetween the two points
						if ((int)y >= (int)laserPoints1[i].y && (int)y <= (int)laserPoints2[i].y && (int)laserPoints1[i].y - (int)laserPoints2[i].y < 0)
						{
							doRender = true;
						}
						else if ((int)y <= (int)laserPoints1[i].y && (int)y >= (int)laserPoints2[i].y && (int)laserPoints1[i].y - (int)laserPoints2[i].y > 0)
						{
							doRender = true;
						}
						if (doRender == true)
						{
							//check distance from laser line
							if (abs(x - laserPoints2[i].x) < laserAreaLightRange)
							{
								int inOutCount = CheckLineForObject((int)laserPoints2[i].x, y, x, y);
								bool isLit = false;
								if (inOutCount <= 1)
								{
									isLit = true;
								}
								if (isLit == true)
								{
									float scaledDist = abs(x - (int)laserPoints2[i].x) / laserAreaLightRange;
									float scaledDistSquared = scaledDist * scaledDist;

									float falloff = 0.8f * (float)pow((1 - scaledDistSquared), 2) / (1 + .5f * scaledDist);
									IntensityR = (IntensityR + falloff * (laserAreaLightColor.r / 255.0f));
									IntensityG = (IntensityG + falloff * (laserAreaLightColor.g / 255.0f));
									IntensityB = (IntensityB + falloff * (laserAreaLightColor.b / 255.0f));

									lightR[x][y] += IntensityR * lightWeight * lightMultiplier;
									lightG[x][y] += IntensityG * lightWeight * lightMultiplier;
									lightB[x][y] += IntensityB * lightWeight * lightMultiplier;
								}

								if (abs(x - laserPoints2[i].x) < laserSize)
								{
									float scaledDistFromCenter = (laserSize - abs(x - (int)laserPoints2[i].x)) / laserSize;
									float tempIntensityR = scaledDistFromCenter * ((float)laserColor[i].r / 255.0f);
									float tempIntensityG = scaledDistFromCenter * ((float)laserColor[i].g / 255.0f);
									float tempIntensityB = scaledDistFromCenter * ((float)laserColor[i].b / 255.0f);
									if (tempIntensityR >= laserThreshold || tempIntensityG >= laserThreshold || tempIntensityB >= laserThreshold)
									{
										lightR[x][y] = tempIntensityR * laserWeight;
										lightG[x][y] = tempIntensityG * laserWeight;
										lightB[x][y] = tempIntensityB * laserWeight;

										Color& DestPixel = outputBuffer->SampleColor(x, y);
										DestPixel.r = clampInt8((uint8_t)(lightR[x][y] * 255), 0, 254);
										DestPixel.g = clampInt8((uint8_t)(lightG[x][y] * 255), 0, 254);
										DestPixel.b = clampInt8((uint8_t)(lightB[x][y] * 255), 0, 254);
									}
								}
							}
						}
					}
				}
			}
		}
	}
}

void Renderer::DrawLaserLines(int thickness)
{
	float thicknessOver2 = (float)(thickness / 2);
	for (int i = 0; i < numLasers; i++)
	{
		for (int j = 0; j < thickness; j++)
		{
			if ((int)laserPoints1[i].x - (int)laserPoints2[i].x == 0)
			{
				DrawLine(Vector2((int)laserPoints1[i].x + j - thicknessOver2, (int)laserPoints1[i].y), Vector2((int)laserPoints2[i].x + j - thicknessOver2, (int)laserPoints2[i].y), laserColor[i], laserBuffer);
			}
			else if ((int)laserPoints1[i].y - (int)laserPoints2[i].y == 0)
			{
				DrawLine(Vector2((int)laserPoints1[i].x, (int)laserPoints1[i].y + j - thicknessOver2), Vector2((int)laserPoints2[i].x, (int)laserPoints2[i].y + j - thicknessOver2), laserColor[i], laserBuffer);
			}
		}
	}
}

void Renderer::RenderMenuSelections()
{
	for (int i = 0; i < numMenuSelections; i++)
	{
		if (menuSelectionType[i] == Pip)
		{
			menuPip.position = menuSelectionPos[i];
			outputBuffer->AddSprite(&menuPip);
		}
		else if (menuSelectionType[i] == Check)
		{
			menuCheck.position = menuSelectionPos[i];
			outputBuffer->AddSprite(&menuCheck);
		}
	}
}

void Renderer::UpdateAnimations(float dt = 0)
{
	if (timer >= timeBetweenFrames)
	{
		for (int i = 0; i < numAnimatedObjects; ++i)
		{
			if (animatedObjects[i][0]->currentFrame == animatedObjects[i][0]->totalFrames)
			{
				animatedObjects[i][0]->currentFrame = 0;
			}
			else
			{
				animatedObjects[i][0]->currentFrame += 1;
			}
		}
		timer = 0;
	}
	else
	{
		timer += dt;
	}
}

//move out into own class
ImageBuffer* Renderer::CreateAnimatedObject(const std::string filename, Vector2 frameSize)
{
	ImageBuffer* Result = NULL;
	ImageBuffer* spriteSheet = new ImageBuffer{ filename };
	if (spriteSheet->buffer && (frameSize.x > 0) && (frameSize.y > 0))
	{
		spriteSheet->position.y = 0;
		ImageBuffer* temp = NULL;

		for (int i = 0; i < spriteSheet->size.x / frameSize.x; i++)
		{
			temp = new ImageBuffer{ frameSize.x, frameSize.y };
			spriteSheet->position.x = -(frameSize.x * i);
			animatedObjects[numAnimatedObjects][i] = &temp->AddSprite(spriteSheet);

			//delete temp;
		}

		animatedObjects[numAnimatedObjects][0]->totalFrames = (int)((spriteSheet->size.x / frameSize.x) - 1);
		numAnimatedObjects += 1;

		Result = animatedObjects[numAnimatedObjects - 1][0];
	}

	delete spriteSheet;
	return(Result);
}

//maybe move into player class or component?
void Renderer::UpdateFace(int faceState_)
{
	//temperory fix for now
	//solution method:
	//adding a face flag in the renderer and a function names TurnoffFace() to set the flag
	//if the flag is true, than do not render any face action but the default emotion
	//else do the normal face rendering
	if (!faceflag)
	{
		if ((faceIndex < 0 || faceIndex > MAX_ANIMATED_OBJECTS) && numAnimatedObjects != 0)
		{
			faceIndex = numAnimatedObjects;
			ImageBuffer* temp = CreateAnimatedObject("./Assets/PPM/Man_Faces.ppm", { 8,8 });
			temp->isCulled = true;
		}
		else if (faceIndex != -1)
		{
			if (faceState_ >= 0 && faceIndex < MAX_ANIMATED_OBJECTS && animatedObjects[faceIndex][0] != NULL && faceState_ <= animatedObjects[faceIndex][0]->totalFrames)
			{
				faceState = faceState_;
				if (animatedObjects[0] != NULL && animatedObjects[0][animatedObjects[0][0]->currentFrame]->isFlipped != animatedObjects[faceIndex][faceState]->isFlipped)
				{
					animatedObjects[faceIndex][faceState]->FlipSprite();
				}
				animatedObjects[0][animatedObjects[0][0]->currentFrame]->AddSprite(animatedObjects[faceIndex][faceState]);
			}
		}
	}
	else
	{
		if ((faceIndex < 0 || faceIndex > MAX_ANIMATED_OBJECTS) && numAnimatedObjects != 0)
		{
			faceIndex = numAnimatedObjects;
			ImageBuffer* temp = CreateAnimatedObject("./Assets/PPM/Man_Faces.ppm", { 8,8 });
			temp->isCulled = true;
		}
		else if (faceIndex != 1)
		{
			if (faceState_ >= 0 && faceIndex < MAX_ANIMATED_OBJECTS && animatedObjects[faceIndex][0] != NULL && faceState_ <= animatedObjects[faceIndex][0]->totalFrames)
			{
				faceState = faceState_;
				if (animatedObjects[0] != NULL && animatedObjects[0][animatedObjects[0][0]->currentFrame]->isFlipped != animatedObjects[faceIndex][faceState]->isFlipped)
				{
					animatedObjects[faceIndex][faceState]->FlipSprite();
				}
				animatedObjects[0][animatedObjects[0][0]->currentFrame]->AddSprite(animatedObjects[faceIndex][faceState]);
			}
		}
	}
}

void Renderer::TintScreenBlack(float progress, Color tintColor)
{
	fadePercent = progress;
	fadeColor = tintColor;
}

//Refactor all these to do all at once when given a master tileset
//using the given name + a suffix to get the normals and shadows
void Renderer::MakeTileMap(int** tileMapArray)
{
	ResizeBuffers();
	tileMap = tileMapArray;

	ImageBuffer* clearTile = new ImageBuffer{ 8,8 };

	for (int x = 0; x < tileMapSize.x; ++x)
	{
		for (int y = 0; y < tileMapSize.y; ++y)
		{
			if (tileSet[tileMapArray[x][y]])
			{
				bool isWall = false;
				for (int i = 0; i < NUM_WALL_TILES; i++)
				{
					if (tileMapArray[x][y] == wallTileIndexes[i])
					{
						isWall = true;
						break;
					}
				}
				if (isWall == false)
				{
					tileSet[tileMapArray[x][y]]->position = { (float)(x * TILE_SIZE), (float)(y * TILE_SIZE) };
					backgroundLayer->AddSprite(tileSet[tileMapArray[x][y]]);

					clearTile->position = { (float)(x * TILE_SIZE), (float)(y * TILE_SIZE) };
					foregroundLayer->AddSprite(clearTile);
				}
				else
				{
					tileSet[tileMapArray[x][y]]->position = { (float)(x * TILE_SIZE), (float)(y * TILE_SIZE) };
					foregroundLayer->AddSprite(tileSet[tileMapArray[x][y]]);

					clearTile->position = { (float)(x * TILE_SIZE), (float)(y * TILE_SIZE) };
					backgroundLayer->AddSprite(clearTile);
				}

				if (normalTileSet[tileMapArray[x][y]])
				{
					normalTileSet[tileMapArray[x][y]]->position = { (float)(x * TILE_SIZE), (float)(y * TILE_SIZE) };
					normalBuffer->AddSprite(normalTileSet[tileMapArray[x][y]]);
				}
				else if (normalTileSet[0])
				{
					normalTileSet[0]->position = { (float)(x * TILE_SIZE), (float)(y * TILE_SIZE) };
					normalBuffer->AddSprite(normalTileSet[0]);
				}

				if (shadowCasterTileset[tileMapArray[x][y]])
				{
					shadowCasterTileset[tileMapArray[x][y]]->position = { (float)(x * TILE_SIZE), (float)(y * TILE_SIZE) };
					shadowCasterBuffer->AddSprite(shadowCasterTileset[tileMapArray[x][y]]);
				}
				else if (shadowCasterTileset[0])
				{
					shadowCasterTileset[0]->position = { (float)(x * TILE_SIZE), (float)(y * TILE_SIZE) };
					shadowCasterBuffer->AddSprite(shadowCasterTileset[0]);
				}
			}
		}
	}
	delete clearTile;
}

void Renderer::AddDecal(ImageBuffer* sprite, Vector2 pos)
{
	if (numDecals < MAX_DECALS)
	{
		decals[numDecals] = sprite;
		decalPositions[numDecals] = pos;
		decalsAreDirty = true;
		numDecals += 1;
		return;
	}
	for (int i = 0; i < numDecals; i++)
	{
		if (decals[i] == NULL)
		{
			decals[i] = sprite;
			decalPositions[i] = pos;
			decalsAreDirty = true;
			return;
		}
	}
}

void Renderer::AddDecalsToTilemap()
{
	for (int i = 0; i < numDecals; i++)
	{
		if (decals[i] != NULL)
		{
			decals[i]->position = decalPositions[i];
			backgroundLayer->AddSprite(decals[i]);
		}
	}
}

//only removes the specified decal from the renderer, call before you delete a given decal
void Renderer::FreeDecalFromRenderer(ImageBuffer* sprite)
{
	for (int i = 0; i < numDecals; i++)
	{
		if (decals[i] == sprite)
		{
			decals[i] = nullptr;

			//decalsAreDirty = true; //possibly add this later if want to redraw tilemap to get rid of removed decals
		}
	}
}

void Renderer::TileMapSetTile(Vector2 pos, int tile)
{
	tileMap[(int)pos.x][(int)pos.y] = tile;
	ImageBuffer* clearTile = new ImageBuffer{ 8,8 };

	bool isWall = false;
	for (int i = 0; i < NUM_WALL_TILES; i++)
	{
		if (tileMap[(int)pos.x][(int)pos.y] == wallTileIndexes[i])
		{
			isWall = true;
			break;
		}
	}
	if (isWall == false)
	{
		tileSet[tileMap[(int)pos.x][(int)pos.y]]->position = { (float)((int)pos.x * TILE_SIZE), (float)((int)pos.y * TILE_SIZE) };
		backgroundLayer->AddSprite(tileSet[tileMap[(int)pos.x][(int)pos.y]]);

		clearTile->position = { (float)((int)pos.x * TILE_SIZE), (float)((int)pos.y * TILE_SIZE) };
		foregroundLayer->AddSpriteIgnoreAlpha(clearTile);
	}
	else
	{
		tileSet[tileMap[(int)pos.x][(int)pos.y]]->position = { (float)((int)pos.x * TILE_SIZE), (float)((int)pos.y * TILE_SIZE) };
		foregroundLayer->AddSprite(tileSet[tileMap[(int)pos.x][(int)pos.y]]);

		clearTile->position = { (float)((int)pos.x * TILE_SIZE), (float)((int)pos.y * TILE_SIZE) };
		backgroundLayer->AddSpriteIgnoreAlpha(clearTile);
	}

	if (normalTileSet[tileMap[(int)pos.x][(int)pos.y]])
	{
		normalTileSet[tileMap[(int)pos.x][(int)pos.y]]->position = { (float)((int)pos.x * TILE_SIZE), (float)((int)pos.y * TILE_SIZE) };
		normalBuffer->AddSpriteIgnoreAlpha(normalTileSet[tileMap[(int)pos.x][(int)pos.y]]);
	}

	if (shadowCasterTileset[tileMap[(int)pos.x][(int)pos.y]])
	{
		shadowCasterTileset[tileMap[(int)pos.x][(int)pos.y]]->position = { (float)((int)pos.x * TILE_SIZE), (float)((int)pos.y * TILE_SIZE) };
		shadowCasterBuffer->AddSpriteIgnoreAlpha(shadowCasterTileset[tileMap[(int)pos.x][(int)pos.y]]);
	}
}

void Renderer::TileMapEraseTile(Vector2 pos)
{
	if (pos.x >= 0 && pos.x < tileMapSize.x && pos.y >= 0 && pos.y < tileMapSize.y)
	{
		tileMap[(int)pos.x][(int)pos.y] = 0;
		ImageBuffer* clearTile = new ImageBuffer{ 8,8 };
		clearTile->position = { (float)((int)pos.x * TILE_SIZE), (float)((int)pos.y * TILE_SIZE) };

		foregroundLayer->AddSpriteIgnoreAlpha(clearTile);

		backgroundLayer->AddSpriteIgnoreAlpha(clearTile);

		normalBuffer->AddSpriteIgnoreAlpha(clearTile);

		shadowCasterBuffer->AddSpriteIgnoreAlpha(clearTile);
	}
}

void Renderer::AddTileToTileset(ImageBuffer* tile)
{
	tileSet[numTiles] = tile;
	numTiles += 1;
}

void Renderer::AddNormalToNormalTileset(ImageBuffer* tile)
{
	normalTileSet[numNormalTiles] = tile;
	numNormalTiles += 1;
}

void Renderer::AddShadowCasterToShadowCasterTileset(ImageBuffer* tile)
{
	shadowCasterTileset[numShadowCasterTiles] = tile;
	numShadowCasterTiles += 1;
}

Renderer::Renderer() : objects{ NULL }
{
	outputBuffer = new ImageBuffer{ SCREEN_SIZE_X ,SCREEN_SIZE_Y };
	lightBuffer = new ImageBuffer{ SCREEN_SIZE_X ,SCREEN_SIZE_Y };
	laserBuffer = new ImageBuffer{ SCREEN_SIZE_X ,SCREEN_SIZE_Y };
	shadowCasterBuffer = new ImageBuffer{ SCREEN_SIZE_X ,SCREEN_SIZE_Y };
	menuBuffer = new ImageBuffer{ SCREEN_SIZE_X ,SCREEN_SIZE_Y };

	particleManager = new ParticleManager;
	faceIndex = -1;
	faceState = 0;
	outputBuffer->screenScale = screenScale;

	// normalBufferPostCam = new ImageBuffer{ SCREEN_SIZE_X ,SCREEN_SIZE_Y };
	//fogBuffer = new ImageBuffer{ SCREEN_SIZE_X ,SCREEN_SIZE_Y };
	//fogBufferPostCam = new ImageBuffer{ SCREEN_SIZE_X ,SCREEN_SIZE_Y };

	for (int i = 0; i < 15; ++i)
	{
		PreviousFrameLengths[i] = 0;
	}

	for (int i = 0; i < MAX_OBJECTS; ++i)
	{
		objects[i] = NULL;
	}

	for (int i = 0; i < MAX_ANIMATED_OBJECTS; ++i)
	{
		for (int j = 0; j < MAX_ANIMATION_FRAMES; ++j)
		{
			animatedObjects[i][j] = NULL;
		}
	}

	for (int i = 0; i < MAX_TILES; ++i)
	{
		tileSet[i] = NULL;
		normalTileSet[i] = NULL;
		shadowCasterTileset[i] = NULL;
	}

	//i hate this fix later
	ReallocateLightArrays();

	startTime = SDL_GetTicks();
	PreviousFrameBeginTime = startTime;
}

//make more clear that its updating the *rendering*
void Renderer::UpdateObjects(float dt)
{
	objectLayer->ClearImageBuffer();
	UpdateAnimations(dt);
	UpdateFace(faceState);

	for (int i = 0; i < numObjects; ++i)
	{
		for (int l = 0; l < 3; ++l)
		{
			if (objects[i])
			{
				if (objects[i]->layer == l && objects[i]->totalFrames == 0 && objects[i]->isCulled == false)
				{
					objectLayer->AddSprite(objects[i]);
					break;
				}
			}
		}
	}
	for (int i = 0; i < numAnimatedObjects; ++i)
	{
		for (int l = 0; l < 3; ++l)
		{
			if (animatedObjects[i][0]->layer == l && animatedObjects[i][0]->isCulled == false)
			{
				int frame = animatedObjects[i][0]->currentFrame;
				animatedObjects[i][frame]->position = animatedObjects[i][0]->position;
				if ((animatedObjects[i][0]->isFlipped == true && animatedObjects[i][frame]->isFlipped != true) || (animatedObjects[i][0]->isFlipped == false && animatedObjects[i][frame]->isFlipped != false))
				{
					animatedObjects[i][frame]->FlipSprite();
				}
				objectLayer->AddSprite(animatedObjects[i][frame]);
				break;
			}
		}
	}
}

//remove when moving objects out
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

void Renderer::LoadMenuPage(int index)
{
	if (currentMenu != index && numMenuPages != 0)
	{
		currentMenu = index;
		menuBuffer->ClearImageBuffer();
		if (index != -1 && menuPages[index] != NULL)
		{
			menuBuffer->AddSprite(menuPages[index]);
		}
	}
}

int Renderer::AddMenuPage(const std::string filename)
{
	if (numMenuPages > MAX_MENU_PAGES)
	{
		return -1;
	}
	for (int i = 0; i < MAX_MENU_PAGES; i++)
	{
		if (menuPages[i] == NULL)
		{
			menuPages[i] = new ImageBuffer{ filename };
			numMenuPages += 1;
			return i;
		}
	}
	return -1;
}

void Renderer::ResetMenuPages()
{
	for (int i = 0; i < MAX_MENU_PAGES; i++)
	{
		if (menuPages[i] != nullptr)
		{
			menuPages[i] = nullptr;
		}
	}
	numMenuPages = 0;
	currentMenu = -1;

	pauseMenuIndex = -1;
	exitConfirmIndex = -1;
	settingsMenuIndex = -1;
	confirmQuitMainindex = -1;
	creditsMainMenuIndex = -1;
	mainMenuOptionsIndex = -1;
	mainMenuHelpIndex = -1;
	mainMenuIndex = -1;
}

void Renderer::AddLight(Light light)
{
	if (numLights + 1 < MAX_LIGHT_SOURCES)
	{
		lightSource[numLights++] = light;
	}
}

void Renderer::AddObject(ImageBuffer* sprite)
{
	if ((numObjects + 1) < MAX_OBJECTS)
	{
		objects[numObjects] = sprite;
		numObjects += 1;
	}
}

//remove when moving objects out
int Renderer::ReturnObjectCount()
{
	int countObjects = 0;
	for (int i = 0; i < MAX_OBJECTS; ++i) {
		if (objects[i] != nullptr) {
			countObjects++;
		}
	}
	return countObjects;
}

void Renderer::ReallocateLightArrays()
{
	if (lightR != NULL)
	{
		//Free indexes
		for (int x = 0; x < SCREEN_SIZE_X; ++x) {
			delete[] lightR[x];
			delete[] lightG[x];
			delete[] lightB[x];
			delete[] blurLightR[x];
			delete[] blurLightG[x];
			delete[] blurLightB[x];
			lightR[x] = NULL;
			lightG[x] = NULL;
			lightB[x] = NULL;
			blurLightR[x] = NULL;
			blurLightG[x] = NULL;
			blurLightB[x] = NULL;
		}

		//Free main array
		delete[] lightR;
		delete[] lightG;
		delete[] lightB;
		delete[] blurLightR;
		delete[] blurLightG;
		delete[] blurLightB;
		lightR = NULL;
		lightG = NULL;
		lightB = NULL;
		blurLightR = NULL;
		blurLightG = NULL;
		blurLightB = NULL;
	}

	lightR = new float* [SCREEN_SIZE_X];
	lightG = new float* [SCREEN_SIZE_X];
	lightB = new float* [SCREEN_SIZE_X];

	for (int x = 0; x < SCREEN_SIZE_X; ++x)
	{
		lightR[x] = new float[SCREEN_SIZE_Y];
		lightG[x] = new float[SCREEN_SIZE_Y];
		lightB[x] = new float[SCREEN_SIZE_Y];
	}

	blurLightR = new float* [SCREEN_SIZE_X];
	blurLightG = new float* [SCREEN_SIZE_X];
	blurLightB = new float* [SCREEN_SIZE_X];

	for (int x = 0; x < SCREEN_SIZE_X; ++x)
	{
		blurLightR[x] = new float[SCREEN_SIZE_Y];
		blurLightG[x] = new float[SCREEN_SIZE_Y];
		blurLightB[x] = new float[SCREEN_SIZE_Y];
	}
}

void Renderer::LoadMenuToBuffer(const std::string filename)
{
	//menuBuffer = new ImageBuffer{filename}; //making new one might delete this
}

Vector2 Renderer::GetCameraPosition(void)
{
	return CameraP;
}

void Renderer::SetCameraPosition(Vector2 NewCameraP)
{
	nextCamPos = NewCameraP;
}

Renderer* Renderer::GetInstance()
{
	if (!instance)
	{
		instance.reset(new Renderer());
	}
	instance->faceflag = false;
	return instance.get();
}

void Renderer::TurnoffFace()
{
	faceflag = true;
}

int Renderer::CheckLineForObject(int x1, int y1, int x2, int y2)
{
	int dx = abs(x2 - x1);
	int dy = abs(y2 - y1);
	int sx = (x1 < x2) ? 1 : -1;
	int sy = (y1 < y2) ? 1 : -1;

	int error = dx - dy;
	int x = x1;
	int y = y1;

	bool inOut = false;
	int inOutCount = 0;

	while (x != x2 || y != y2)
	{
		if (inOut == false)
		{
			if (shadowCasterBuffer->SampleColor(x + (int)CameraP.x, y + (int)CameraP.y).GetAlpha() != 0)
			{
				inOutCount += 1;
				inOut = true;
			}
		}
		else
		{
			if (shadowCasterBuffer->SampleColor(x + (int)CameraP.x, y + (int)CameraP.y).GetAlpha() == 0)
			{
				inOutCount += 1;
				inOut = false;
			}
		}

		if (inOutCount > 1)
		{
			return 2;
		}

		int error2 = 2 * error;

		if (error2 > -dy) {
			error -= dy;
			x += sx;
		}

		if (error2 < dx) {
			error += dx;
			y += sy;
		}
	}
	return inOutCount;
}

gfxVector2 Renderer::LaserCheckLineForObject(Vector2 pos1, Vector2 pos2)
{
	int dx = (int)abs(pos2.x - pos1.x);
	int dy = (int)abs(pos2.y - pos1.y);
	int sx = (pos1.x < pos2.x) ? 1 : -1;
	int sy = (pos1.y < pos2.y) ? 1 : -1;
	gfxVector2 result;
	int error = dx - dy;
	int x = (int)pos1.x;
	int y = (int)pos1.y;

	int currIterations = 0;

	while (x != (int)pos2.x || y != (int)pos2.y)
	{
		if (currIterations > 10000)//if its looking for more then 10k times then just stop
		{
			return pos2;
		}
		if (shadowCasterBuffer->SampleColor(x + (int)CameraP.x, y + (int)CameraP.y).GetAlpha() != 0)
		{
			result.x = (float)x;
			result.y = (float)y;
			return result;
		}

		int error2 = 2 * error;

		if (error2 > -dy) {
			error -= dy;
			x += sx;
		}

		if (error2 < dx) {
			error += dx;
			y += sy;
		}
		currIterations += 1;
	}

	return pos2;
}

void Renderer::BlurLights(int blurRangeLow, int blurRangeHigh)
{
	if (doBlur == true)
	{
		const int xSize = (int)outputBuffer->size.x;
		const int ySize = (int)outputBuffer->size.y;

#pragma omp parallel for collapse(2)
		for (int x = 0; x < xSize; ++x)
		{
			for (int y = 0; y < ySize; ++y)
			{
				blurLightR[x][y] = lightR[x][y];
				blurLightG[x][y] = lightG[x][y];
				blurLightB[x][y] = lightB[x][y];
			}
		}

		int count;
#pragma omp parallel for collapse(2) private(count)
		for (int x = 0; x < xSize; ++x)
		{
			for (int y = 0; y < ySize; ++y)
			{
				lightR[x][y] = 0.0f;
				lightG[x][y] = 0.0f;
				lightB[x][y] = 0.0f;

				count = 0;
				for (int w = blurRangeLow; w < blurRangeHigh; ++w)
				{
					for (int h = blurRangeLow; h < blurRangeHigh; ++h)
					{
						if (x + w < SCREEN_SIZE_X && y + h < SCREEN_SIZE_Y && x + w >= 0 && y + h >= 0)
						{
							lightR[x][y] += blurLightR[x + w][y + h];
							lightG[x][y] += blurLightG[x + w][y + h];
							lightB[x][y] += blurLightB[x + w][y + h];
							count += 1;
						}
					}
				}
				lightR[x][y] /= count;
				lightG[x][y] /= count;
				lightB[x][y] /= count;
			}
		}
	}
}

void Renderer::BlurLasers(int blurRangeLow, int blurRangeHigh)
{
	const int xSize = (int)laserBuffer->size.x;
	const int ySize = (int)laserBuffer->size.y;

	int count;
	float r = 0;
	float g = 0;
	float b = 0;

	//not actually unreferenced but W4 gets pissy
	UNREFERENCED_PARAMETER(r);
	UNREFERENCED_PARAMETER(g);
	UNREFERENCED_PARAMETER(b);

	if (doBlur)
	{
#pragma omp parallel for collapse(2) private(count, r, g, b)
		for (int x = 0; x < xSize; ++x)
		{
			for (int y = 0; y < ySize; ++y)
			{
				r = 0;
				g = 0;
				b = 0;
				count = 0;
#pragma omp parallel for collapse(2) shared(count,r,g,b)
				for (int w = blurRangeLow; w < blurRangeHigh; ++w)
				{
					for (int h = blurRangeLow; h < blurRangeHigh; ++h)
					{
						if (x + w < SCREEN_SIZE_X && y + h < SCREEN_SIZE_Y && x + w >= 0 && y + h >= 0)
						{
							Color& curPixel = laserBuffer->SampleColor(x + w, y + h);
							r += curPixel.r;
							g += curPixel.g;
							b += curPixel.b;
							count += 1;
						}
					}
				}
				r /= count;
				g /= count;
				b /= count;

				lightBuffer->SampleColor(x, y) = Color((uint8_t)r, (uint8_t)g, (uint8_t)b, 255);
			}
		}
	}
	else
	{
#pragma omp parallel for collapse(2) private(count, r, g, b)
		for (int x = 0; x < xSize; ++x)
		{
			for (int y = 0; y < ySize; ++y)
			{
				lightBuffer->SampleColor(x, y) = laserBuffer->SampleColor(x, y);
			}
		}
	}
#pragma omp parallel for collapse(2) private(count, r, g, b)
	for (int x = 0; x < xSize; ++x)
	{
		for (int y = 0; y < ySize; ++y)
		{
			laserBuffer->SampleColor(x, y) = lightBuffer->SampleColor(x, y);
		}
	}
}

Renderer::~Renderer(void)
{
	CleanRenderer();
	delete outputBuffer;
	delete objectLayer;
	delete backgroundLayer;
	delete foregroundLayer;
	delete normalBuffer;
	delete DebugBuffer;
	delete lightBuffer;
	delete particleManager;
	delete shadowCasterBuffer;
	delete laserBuffer;
	delete menuBuffer;

	outputBuffer = NULL;
	objectLayer = NULL;
	backgroundLayer = NULL;
	foregroundLayer = NULL;
	normalBuffer = NULL;
	DebugBuffer = NULL;
	lightBuffer = NULL;
	particleManager = NULL;
	shadowCasterBuffer = NULL;
	laserBuffer = NULL;
	menuBuffer = NULL;

	//Free indexes
	for (int x = 0; x < SCREEN_SIZE_X; ++x) {
		delete[] lightR[x];
		delete[] lightG[x];
		delete[] lightB[x];
		delete[] blurLightR[x];
		delete[] blurLightG[x];
		delete[] blurLightB[x];
		lightR[x] = NULL;
		lightG[x] = NULL;
		lightB[x] = NULL;
		blurLightR[x] = NULL;
		blurLightG[x] = NULL;
		blurLightB[x] = NULL;
	}

	//Free main array
	delete[] lightR;
	delete[] lightG;
	delete[] lightB;
	delete[] blurLightR;
	delete[] blurLightG;
	delete[] blurLightB;
	lightR = NULL;
	lightG = NULL;
	lightB = NULL;
	blurLightR = NULL;
	blurLightG = NULL;
	blurLightB = NULL;

	glDeleteTextures(1, &OutputBufferTexture);
}

void Renderer::CleanRenderer()
{
	outputBuffer->ClearImageBuffer();
	objectLayer->ClearImageBuffer();
	backgroundLayer->ClearImageBuffer();
	foregroundLayer->ClearImageBuffer();
	normalBuffer->ClearImageBuffer();
	lightBuffer->ClearImageBuffer();
	laserBuffer->ClearImageBuffer();
	menuBuffer->ClearImageBuffer();
	laserHandler.Clear();
	ClearMenuPages();
	particleManager->ClearParticles();
	ReallocateLightArrays();
	ClearTilesets();

	fadePercent = 1;
	faceIndex = -1;
	numTiles = 0;
	numNormalTiles = 0;
	numObjects = 0;
	numAnimatedObjects = 0;
	numLights = 0;
	frameCount = 0;
	numLasers = 0;
	timer = 0;
	CameraP = Vector2{ 0,0 };
	numMenuSelections = 0;
}

void Renderer::ClearSprites()
{
	for (int i = 0; i < numObjects; ++i)
	{
		if (objects[i] != NULL)
		{
			delete objects[i];
			objects[i] = NULL;
		}
	}
	numObjects = 0;
	for (int i = 0; i < numAnimatedObjects; ++i)
	{
		if (objects[i] != NULL)
		{
			for (int f = 0; f < MAX_ANIMATION_FRAMES; ++f)
			{
				delete animatedObjects[i][f];
				animatedObjects[i][f] = NULL;
			}
		}
	}
	numAnimatedObjects = 0;
	faceIndex = -1;
}

void Renderer::ClearTilesets()
{
	for (int i = 0; i < MAX_TILES; i++)
	{
		if (tileSet[i] != NULL)
		{
			delete tileSet[i];
			tileSet[i] = NULL;
		}
		if (normalTileSet[i] != NULL)
		{
			delete normalTileSet[i];
			normalTileSet[i] = NULL;
		}
		if (shadowCasterTileset[i] != NULL)
		{
			delete shadowCasterTileset[i];
			shadowCasterTileset[i] = NULL;
		}
	}
	numTiles = 0;
	numNormalTiles = 0;
	numShadowCasterTiles = 0;
}

void Renderer::ClearMenuPages()
{
	for (int i = 0; i < MAX_MENU_PAGES; i++)
	{
		if (menuPages[i] != NULL)
		{
			delete menuPages[i];
			menuPages[i] = NULL;
		}
	}
	numMenuPages = 0;
}

void Renderer::ExpandTileMapInDirection(Vector2 direction, int distance)
{
	int** tempTileMap = NULL;
	Vector2 newTileMapSize = Vector2{ tileMapSize.x + (abs(direction.x) * distance), tileMapSize.y + (abs(direction.y) * distance) };
	tempTileMap = new int* [(size_t)newTileMapSize.x];

	for (int x = 0; x < newTileMapSize.x; ++x)
	{
		tempTileMap[x] = new int[(size_t)newTileMapSize.y];
	}

	for (int x = 0; x < (int)newTileMapSize.x; ++x)
	{
		for (int y = 0; y < (int)newTileMapSize.y; ++y)
		{
			tempTileMap[x][y] = 0;
		}
	}

	for (int x = 0; x < (int)tileMapSize.x; ++x)
	{
		for (int y = 0; y < (int)tileMapSize.y; ++y)
		{
			if (direction.x >= 0 && direction.y >= 0)
			{
				tempTileMap[x][y] = tileMap[x][y];
			}
			if (direction.x * distance + x >= 0 && direction.y * distance + y >= 0 && direction.x * distance + x <= tileMapSize.x && direction.y * distance + y <= tileMapSize.y)
			{
				if (direction.x < 0 || direction.y < 0)
				{
					tempTileMap[x][y] = tileMap[(x + (int)(direction.x * distance))][(y + (int)(direction.y * distance))];
				}
			}
		}
	}

	if (tileMap != NULL)
	{
		for (int x = 0; x < tileMapSize.x; ++x)
		{
			if (tileMap[x] != NULL)
			{
				delete[] tileMap[x];
			}
		}
		delete[] tileMap;
	}

	tileMapSize = newTileMapSize;
	tileMap = tempTileMap;
	ResizeBuffers();
}

auto Renderer::GetTileCount() -> unsigned int
{
	return numTiles;
}

void Renderer::ResizeBuffers()
{
	if (backgroundLayer != NULL)
	{
		delete objectLayer;
		delete backgroundLayer;
		delete foregroundLayer;
		delete normalBuffer;
		delete shadowCasterBuffer;
		delete DebugBuffer;
	}

	objectLayer = new ImageBuffer{ tileMapSize.x * (TILE_SIZE), tileMapSize.y * (TILE_SIZE) };
	backgroundLayer = new ImageBuffer{ tileMapSize.x * (TILE_SIZE), tileMapSize.y * (TILE_SIZE) };
	foregroundLayer = new ImageBuffer{ tileMapSize.x * (TILE_SIZE), tileMapSize.y * (TILE_SIZE) };
	normalBuffer = new ImageBuffer{ tileMapSize.x * (TILE_SIZE), tileMapSize.y * (TILE_SIZE) };
	shadowCasterBuffer = new ImageBuffer{ tileMapSize.x * (TILE_SIZE), tileMapSize.y * (TILE_SIZE) };
	DebugBuffer = new ImageBuffer{ tileMapSize.x * (TILE_SIZE), tileMapSize.y * (TILE_SIZE) };
}

void Renderer::DrawLine(Vector2 P0, Vector2 P1, const Color& LineColor, ImageBuffer* buffer, float skipPercent)
{
	int MinX = (int)min(P0.x, P1.x);
	int MinY = (int)min(P0.y, P1.y);
	int MaxX = (int)max(P0.x, P1.x);
	int MaxY = (int)max(P0.y, P1.y);

	int ClippedMinX = MinX;
	int ClippedMaxX = MaxX;
	int ClippedMinY = MinY;
	int ClippedMaxY = MaxY;
	if (ClippedMinX < 0)
	{
		ClippedMinX = 0;
	}
	if (ClippedMinY < 0)
	{
		ClippedMinY = 0;
	}
	if (ClippedMaxX > ((int)DebugBuffer->size.x - 1))
	{
		ClippedMaxX = (int)DebugBuffer->size.x - 1;
	}
	if (ClippedMaxY > ((int)DebugBuffer->size.y - 1))
	{
		ClippedMaxY = (int)DebugBuffer->size.y - 1;
	}

	Vector2 D = P1 - P0;
	Vector2 N = Vector2::Normalize(Vector2(-D.y, D.x));
	for (int PixelY = ClippedMinY; PixelY <= ClippedMaxY; ++PixelY)
	{
		for (int PixelX = ClippedMinX; PixelX <= ClippedMaxX; ++PixelX)
		{
			Vector2 PixelD = Vector2((float)PixelX + 0.5f, (float)PixelY + 0.5f) - P0;
			float d = fabsf(Vector2::DotProduct(N, PixelD));
			if (d <= 0.5f)
			{
				if (skipPercent == 0 || (skipPercent * 100) < rand() % 100)
				{
					Color& DestPixel = buffer->SampleColor(PixelX, PixelY);
					DestPixel = LineColor;
				}
			}
		}
	}
}

void Renderer::RenderWallCollidersToDebugBuffer()
{
	DebugBuffer->ClearImageBuffer();

	//make debug tile here
	ImageBuffer* debugOutlineTile = new ImageBuffer{ 8,8 };
	Color* temp;
	for (int x = 0; x < debugOutlineTile->size.x; ++x)
	{
		for (int y = 0; y < debugOutlineTile->size.y; ++y)
		{
			if (x == 0 || x == debugOutlineTile->size.x - 1 || y == debugOutlineTile->size.y - 1 || y == 0)
			{
				temp = &debugOutlineTile->SampleColor(x, y);
				*temp = Color{ 255, 0, 0, 255 };
			}
		}
	}

	const int xSize = (int)tileMapSize.x;
	const int ySize = (int)tileMapSize.y;

	//for some reason paralell makes it freak out so imma not do that even though it makes preformance a bit worse
	//#pragma omp parallel for collapse(2)
	for (int x = 0; x < xSize; ++x)
	{
		for (int y = 0; y < ySize; ++y)
		{
			bool isWalkable = true;
			for (int i = 0; i < NUM_NON_WALKABLE_TILES; i++)
			{
				if (tileMap[x][y] == nonWalkableTiles[i])
				{
					isWalkable = false;
					break;
				}
			}
			if (isWalkable == false)
			{
				debugOutlineTile->position = { (float)(x * TILE_SIZE), (float)(y * TILE_SIZE) };
				DebugBuffer->AddSprite(debugOutlineTile);
			}
		}
	}

	debugOutlineTile->position = animatedObjects[0][0]->position;
	DebugBuffer->AddSprite(debugOutlineTile);
}