//------------------------------------------------------------------------------
//
// File Name:	Time.cpp
// Author(s):	TayLee Young, Michael Howard
// Purpose:		Calculation of delta time.
//
// Copyright  � 2023 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------
#include "Time_TM.h"
#include <SDL/SDL.h>

Time::Time()
{
	last_tick_time = 0;
	delta = 0;
}

Time::~Time()
{
}

float Time::Delta()
{
	float tick_time = static_cast<float>(SDL_GetTicks());
	delta = tick_time - last_tick_time;
	last_tick_time = tick_time;
	delta /= 1000;
	if (delta < 0.0f)
	{
		return 0.0f;
	}
	return delta;
}

float Time::GetDt()
{
	return delta;
}

Time& Time::Instance()
{
	static Time instance;
	return instance;
}