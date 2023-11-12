#include "Time.h"
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
	if (delta <= 1.0f && delta >= 0.0f)
	{
		return delta;
	}
	else if (delta < 0.0f)
	{
		return 0.0f;
	}
	else
	{
		return 1.0f;
	}
    //delta = clamp(delta, 0.0f, 1.0f);
}

Time& Time::Instance()
{
	static Time instance;
	return instance;
}