/*!
@file    FrameRate.h
@author	 Louis Wang

Frame related functions like average fps, updating the window and capping of fps

// Copyright © 2023 DigiPen (USA) Corporation.
*//*__________________________________________________________________________*/
#pragma once

#include "Renderer.h"
#include <stdio.h>
#include <thread>

class FrameRate
{
public:
	static void capFrame(float dt);

	static float CalculateAverageFrameRate(float* PreviousFrameLengths, uint32_t arraySize, float currentTime, float PreviousFrameBeginTime);

	static void UpdateWindowTitle(SDL_Window* window, float AverageFrameRate);
};
