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
