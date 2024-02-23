//------------------------------------------------------------------------------
//
// File Name:	Vector.cpp
// Author(s):	Louis Wang
// Purpose:		Vector definition file for all 2D vector calculations
//
// Copyright  © 2023 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------
#include "Vector.h"

gfxVector2::gfxVector2() : x(), y() {};
gfxVector2::gfxVector2(float x, float y) : x(x), y(y)
{
};

//arithmetic operators
gfxVector2 operator+(const gfxVector2& l, const gfxVector2& r)
{
	gfxVector2 result;
	result.x = l.x + r.x;
	result.y = l.y + r.y;
	return result;
}
gfxVector2 operator-(const gfxVector2& l, const gfxVector2& r)
{
	gfxVector2 result;
	result.x = l.x - r.x;
	result.y = l.y - r.y;
	return result;
}
gfxVector2 operator*(float l, const gfxVector2& r)
{
	gfxVector2 result;
	result.x = l * r.x;
	result.y = l * r.y;
	return result;
}
gfxVector2 operator*(const gfxVector2& r, float l)
{
	gfxVector2 result;
	result.x = r.x * l;
	result.y = r.y * l;
	return result;
}

//find length of vector
float length(const gfxVector2& v)
{
	return sqrt(v.x * v.x + v.y * v.y);
}

//dot product between 2 vectors
float operator*(const gfxVector2& l, const gfxVector2& r)
{
	return (l.x * r.x + l.y * r.y);
}

//cross product between 2 vectors
float operator^(const gfxVector2& l, const gfxVector2& r)
{
	return (l.x * r.x - l.y * r.y);
}

//normalization
void Normalize(gfxVector2& vecResult, const gfxVector2 vec)
{
	vecResult.x = vec.x / length(vec);
	vecResult.y = vec.y / length(vec);
}

inline float DotProduct(const gfxVector2& a, const gfxVector2& b)
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