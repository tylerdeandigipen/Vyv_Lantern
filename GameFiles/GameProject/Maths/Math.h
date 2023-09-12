/*!
@file    Math.h
@author	 Louis Wang

Maths functions like min max, special value like PI and deg->rad conversions
*//*__________________________________________________________________________*/

#ifndef MATH_H_
#define MATH_H_

/*                                                                   includes
----------------------------------------------------------------------------- */
#include "Math_Header.h"


#undef min
#undef max

/*                                                                  constants
----------------------------------------------------------------------------- */

const float PI = 4.0f * std::atan(1.0f);
const float DEG_TO_RAD = PI / 180.0f;
const float RAD_TO_DEG = 180.0f / PI;
const float EPSILON = 0.00000001f; // ϵ for error checking

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

//additional class for integers dividing by 0
class ZeroDivision : public std::exception
{
public:
	const char* what()
	{
		return "Divide by zero!";
	}
};
#endif // !MATH_H_