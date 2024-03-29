//------------------------------------------------------------------------------
//
// File Name:	ScopeTimer.h
// Author(s):	TayLee Young
// Purpose:		Timer for debugs
//
// Copyright � 2023 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------
#pragma once
#include <stdlib.h>
#include <iostream>
#include <chrono>
#include <string>

class ScopeTimer
{
	std::chrono::time_point<std::chrono::high_resolution_clock> startTimept;
	std::string name_;

public:
    ScopeTimer(std::string name)
	{
        name_ = name;
		startTimept = std::chrono::high_resolution_clock::now();
	}

	~ScopeTimer()
	{
		//auto endTimept = std::chrono::high_resolution_clock::now();

		//auto start = std::chrono::time_point_cast<std::chrono::microseconds>(startTimept).time_since_epoch().count();
		//auto end = std::chrono::time_point_cast<std::chrono::microseconds>(endTimept).time_since_epoch().count();
		//auto duration = end - start;
		//double microseconds_elapsed = duration * 0.001f;

        // @TODO: Consider some kind of visual profiler to output this to.
        //Logging &Logger = Logging::GetInstance("debugLog.log");
        //Logger.LogLine("SCOPED TIMER (%s): %f", name_.c_str(), microseconds_elapsed);
	}
};
