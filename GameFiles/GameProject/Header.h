#pragma once
#include <stdlib.h>
#include <iostream>
#include <chrono>
#include <string>

class scopeClass
{
	std::chrono::time_point<std::chrono::high_resolution_clock> startTimept;
	std::string name_;
	scopeClass(std::string name)
	{
		startTimept = std::chrono::high_resolution_clock::now();
	}

	~scopeClass()
	{
		auto endTimept = std::chrono::high_resolution_clock::now();

		auto start = std::chrono::time_point_cast<std::chrono::microseconds>(startTimept).time_since_epoch().count();
		auto end = std::chrono::time_point_cast<std::chrono::microseconds>(endTimept).time_since_epoch().count();
		auto duration = end - start;
		double ms = duration * 0.001;


	}
};
