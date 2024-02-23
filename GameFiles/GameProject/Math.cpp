//------------------------------------------------------------------------------
//
// File Name:	Math.cpp
// Author(s):	Louis Wang
// Purpose:		Maths functions like min max, special value like PI and deg->rad conversions
//
// Copyright  © 2023 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "Math.h"

/*                                                                  functions
----------------------------------------------------------------------------- */

//finding the min between 2 elements
template<typename T_>
inline T_ min(const T_& l, const T_& r)
{
	return(l < r ? l : r);
}

//finding the max between 2 elements
template<typename T_>
inline T_ max(const T_& l, const T_& r)
{
	return(l > r ? l : r);
}

//convert degree to radians
inline float deg2rad(float d)
{
	return (d * DEG_TO_RAD);
}

//convert radians to deg
inline float rad2deg(float r)
{
	return (r * RAD_TO_DEG);
}

inline double increment(double r)
{
	return r++;
}

//initialize a random number using srand() and time()
/*The pseudo-random number generator should not be seeded every time we generate a new set of numbers i.e. it should be seeded only once at the beginning of the program, before any calls of rand().
It is preferred to use the result of a call to time(0) as the seed. The time() function returns the number of seconds since 00:00 hours, Jan 1, 1970 UTC (i.e. the current unix timestamp).

As a result, the value of seed changes with time. So every time we run the program, a new set of random numbers is generated.*/
inline void rand_generator()
{
	srand(static_cast<unsigned int>(time(NULL)));
}

//More Uniform way of randomly generating a number
template<typename T>
inline T rand_generator(T start, T end, unsigned int size)
{
	std::default_random_engine generator;
	std::uniform_real_distribution<T> distributor(start, end);

	generator.seed(size);

	return distributor(generator);
}

//random int
inline unsigned IntRand()
{
	return rand();
}

// calculate distance from 2 points
float distance(float x1, float y1, float x2, float y2)
{
	return static_cast<float>(sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2) * 1.0));
}

// calculate distance from 2 points squared
float distanceSquared(float x1, float y1, float x2, float y2)
{
	return static_cast<float>(pow(x2 - x1, 2) + pow(y2 - y1, 2) * 1.0);
}

float clamp(float value, float min, float max)
{
	if (value <= max && value >= min)
	{
		return value;
	}
	else if (value < min)
	{
		return min;
	}
	else
	{
		return max;
	}
}

uint8_t clampInt8(uint8_t value, uint8_t min, uint8_t max)
{
	if (value <= max && value >= min)
	{
		return value;
	}
	else if (value < min)
	{
		return min;
	}
	else
	{
		return max;
	}
}

/*Lerping*/
template<typename T>
inline T lerp(const T& start, const T& end, float t)
{
	return start * (1 - t) + end * t;
}

/*Absolute value*/
template<typename T>
inline T abs(const T& value)
{
	return value < 0 ? -value : value;
}

/*squared*/
template<typename T>
inline T square(const T& value)
{
	return value * value;
}

/*round off*/
template<typename T>
inline T floor(const T& value)
{
	return static_cast<T>(std::floor(value));
}

template<typename T>
inline T ceil(const T& value)
{
	return static_cast<T>(std::ceil(value));
}

template<typename T>
inline T round(const T& value)
{
	return static_cast<T>(std::round(value));
}