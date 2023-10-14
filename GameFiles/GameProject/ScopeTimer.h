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
		startTimept = std::chrono::high_resolution_clock::now();
	}

	~ScopeTimer()
	{
		auto endTimept = std::chrono::high_resolution_clock::now();

		auto start = std::chrono::time_point_cast<std::chrono::microseconds>(startTimept).time_since_epoch().count();
		auto end = std::chrono::time_point_cast<std::chrono::microseconds>(endTimept).time_since_epoch().count();
		auto duration = end - start;
		double ms = duration * 0.001;

		//add debug out code here
	}
};
