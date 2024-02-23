/*!
@file    Vector.h
@author	 Louis Wang

Vector header file for all 2D vector calculations

// Copyright © 2023 DigiPen (USA) Corporation.
*//*__________________________________________________________________________*/

#ifndef VECTOR_H_
#define VECTOR_H_

/*                                                                   includes
----------------------------------------------------------------------------- */
#include "Math_Header.h"

/*                                                                    classes
----------------------------------------------------------------------------- */
class gfxVector2
{
public:

	//constructors & destructors
	gfxVector2();
	gfxVector2(float x, float y);
	gfxVector2(const gfxVector2& v) : x(v.x), y(v.y) {};
	~gfxVector2() { /*delete[] &mVec[2];*/ };

	gfxVector2(gfxVector2&&) = default;

	//operator = assignment
	gfxVector2& operator=(const gfxVector2& v)
	{
		this->x = v.x;
		this->y = v.y;
		return *this;
	}

	//arithmetic operators
	friend gfxVector2 operator+(const gfxVector2& l, const gfxVector2& r);
	friend gfxVector2 operator-(const gfxVector2& l, const gfxVector2& r);
	friend gfxVector2 operator*(float l, const gfxVector2& r);
	friend gfxVector2 operator*(const gfxVector2& r, float l);

	//dot product between 2 vectors
	friend float operator*(const gfxVector2& l, const gfxVector2& r);

	//cross product between 2 vectors
	friend float operator^(const gfxVector2& l, const gfxVector2& r);

	//arithmetic vector operations
	gfxVector2& operator+=(const gfxVector2& v)
	{
		this->x += v.x;
		this->y += v.y;
		return *this;
	}

	// Overloading the equality operator (==)
	bool operator==(const gfxVector2& other) const {
		return (x == other.x && y == other.y);
	}

	gfxVector2& operator-=(const gfxVector2& v)
	{
		this->x -= v.x;
		this->y -= v.y;
		return *this;
	}

	//arithmetic scalar operations
	gfxVector2& operator*=(float i)
	{
		this->x *= i;
		this->y *= i;
		return *this;
	}
	gfxVector2& operator/=(float i)
	{
		this->x /= i;
		this->y /= i;
		return *this;
	}

	//arithmetic negation
	gfxVector2 operator-(void)
	{
		this->x = -x;
		this->y = -y;
		return *this;
	}

	//find length of vector
	float length(const gfxVector2& v) const
	{
		return sqrt(v.x * v.x + v.y * v.y);
	}

	//normalization
	void Normalize(gfxVector2& vecResult, const gfxVector2 vec)
	{
		vecResult.x = vec.x / length(vec);
		vecResult.y = vec.y / length(vec);
	}

	static inline float DotProduct(const gfxVector2& a, const gfxVector2& b)
	{
		float Result = a * b;
		return(Result);
	}

	static inline float Length(const gfxVector2& v)
	{
		float Result = sqrtf(DotProduct(v, v));
		return(Result);
	}

	static inline gfxVector2 Normalize(const gfxVector2& vec)
	{
		gfxVector2 Result = {};
		Result.x = vec.x / Length(vec);
		Result.y = vec.y / Length(vec);
		return(Result);
	}

	static inline gfxVector2 Perp(const gfxVector2& vec)
	{
		gfxVector2 Result = { -vec.y, vec.x };
		return(Result);
	}

	//delegates
	float x; //access to X component
	float y; //access to Y component

private:

	//data member
	float mVec[2]{};
};

typedef gfxVector2 Vector2;

#endif // !VECTOR_H_
