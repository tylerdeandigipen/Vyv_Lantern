//------------------------------------------------------------------------------
//
// File Name:	Color.cpp
// Author(s):	Tyler Dean
// Purpose:		Color class implementation for handling RGBA color values.
//
// Copyright © 2023 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------
#include "Color.h"
#include "Math.h"
#include <assert.h>

float clamp(float value, float min, float max);

Color::Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a) // constructor definition
	: r{ r }
	, g{ g }
	, b{ b }
	, a{ a }
{
}

Color::Color(Color& rhs) 
	: r{ rhs.r }
	, g{ rhs.g }
	, b{ rhs.b }
	, a{ rhs.a }
{
}

Color::Color() // default constructor definition
	: r{ 0 }
	, g{ 0 }
	, b{ 0 }
	, a{ 0 }
{
}

uint8_t Color::GetRed()
{
	if (this != NULL && this->r != NULL)
	{
		return r;
	}
}

uint8_t Color::GetGreen()
{
	if (this != NULL && this->g != NULL)
	{
		return g;
	}
}

uint8_t Color::GetBlue()
{
	if (this != NULL && this->b != NULL)
	{
		return b;
	}
}

uint8_t Color::GetAlpha()
{
	if (this != NULL && this->a != NULL)
	{
		return a;
	}
}

void Color::SetAlpha(uint8_t a_)
{
	if (this != NULL && this->a != NULL)
	{
		a = a_;
	}
}

Color Color::BlendColors(Color top, Color bottom, float blendPercent)
{
	assert(this != NULL);
	Color result;
	if (bottom.r >= top.r)
	{
		result.r = (uint8_t)((bottom.r - top.r) * (blendPercent * 0.01) + top.r);
	}
	else
		result.r = (uint8_t)((top.r - bottom.r) * ((100 - blendPercent) * 0.01) + bottom.r);
	if (bottom.g >= top.g)
	{
		result.g = (uint8_t)((bottom.g - top.g) * (blendPercent * 0.01) + top.g);
	}
	else
		result.g = (uint8_t)((top.g - bottom.g) * ((100 - blendPercent) * 0.01) + bottom.g);
	if (bottom.b >= top.b)
	{
		result.b = (uint8_t)((bottom.b - top.b) * (blendPercent * 0.01) + top.b);
	}
	else
		result.b = (uint8_t)((top.b - bottom.b) * ((100 - blendPercent) * 0.01) + bottom.b);

	return result;
}


#define  Pr  .299
#define  Pg  .587
#define  Pb  .114

//  public-domain function by Darel Rex Finley
//
//  The passed-in RGB values can be on any desired scale, such as 0 to
//  to 1, or 0 to 255.  (But use the same scale for all three!)
//
//  The "change" parameter works like this:
//    0.0 creates a black-and-white image.
//    0.5 reduces the color saturation by half.
//    1.0 causes no change.
//  Note:  A "change" value greater than 1.0 may project your RGB values
//  beyond their normal range, in which case you probably should truncate
//  them to the desired range before trying to use them in an image.

void Color::ChangeSaturation(double percent)
{
	assert(this != NULL);
	double  P = sqrt((r) * (r) * Pr + (g) * (g) * Pg + (b) * (b) * Pb);

	r = (uint8_t)(P + ((r) - P) * percent);
	g = (uint8_t)(P + ((g) - P) * percent);
	b = (uint8_t)(P + ((b) - P) * percent);
}

Color Color::ScaleIndividual(float rScale, float gScale, float bScale)
{
	assert(this != NULL);
	Color result;
	result.r = (uint8_t)clamp((float)this->r * rScale, 0.0f, 255.0f);
	result.g = (uint8_t)clamp((float)this->g * gScale, 0.0f, 255.0f);
	result.b = (uint8_t)clamp((float)this->b * bScale, 0.0f, 255.0f);
	return result;
}
bool Color::operator ==(const Color& rhs)&
{
	assert(this != NULL);
	if (r == rhs.r && g == rhs.g && b == rhs.b)
	{
		return true;
	}
	return false;
}
bool Color::operator !=(const Color& rhs)&
{
	assert(this != NULL);
	if (r != rhs.r || g != rhs.g || b != rhs.b)
	{
		return true;
	}
	return false;
}

Color& Color::operator =(Color const& rhs)& 
{
	assert(this != NULL);
	r = rhs.r;
	g = rhs.g;
	b = rhs.b;
	a = rhs.a;
	return *this;
};

Color& Color::operator +=(Color const& rhs)& {
	assert(this != NULL);
	r = (uint8_t)clamp((float)(r + rhs.r), 0.0f, 255.0f);
	g = (uint8_t)clamp((float)(g + rhs.g), 0.0f, 255.0f);
	b = (uint8_t)clamp((float)(b + rhs.b), 0.0f, 255.0f);
	return *this;
};
Color Color::operator +(const Color& rhs) {
	assert(this != NULL);
	Color result;
	result.r = (uint8_t)clamp((float)(this->r + rhs.r), 0.0f, 255.0f);
	result.g = (uint8_t)clamp((float)(this->g + rhs.g), 0.0f, 255.0f);
	result.b = (uint8_t)clamp((float)(this->b + rhs.b), 0.0f, 255.0f);

	return result;
};
Color& Color::operator -=(Color const& rhs)& {
	assert(this != NULL);
	r = (uint8_t)clamp((float)(r - rhs.r), 0.0f, 255.0f);
	g = (uint8_t)clamp((float)(g - rhs.g), 0.0f, 255.0f);
	b = (uint8_t)clamp((float)(b - rhs.b), 0.0f, 255.0f);
	return *this;
};
Color Color::operator -(const Color& rhs) {
	assert(this != NULL);
	Color result;
	result.r = (uint8_t)clamp((float)(this->r - rhs.r), 0.0f, 255.0f);
	result.g = (uint8_t)clamp((float)(this->g - rhs.g), 0.0f, 255.0f);
	result.b = (uint8_t)clamp((float)(this->b - rhs.b), 0.0f, 255.0f);
	return result;
};
Color Color::operator *(const float rhs) {
	assert(this != NULL);
	Color result;
	result.r = (uint8_t)clamp((float)(this->r * rhs), 0.0f, 255.0f);
	result.g = (uint8_t)clamp((float)(this->g * rhs), 0.0f, 255.0f);
	result.b = (uint8_t)clamp((float)(this->b * rhs), 0.0f, 255.0f);
	return result;
};
Color Color::operator *(const Color& rhs) {
	assert(this != NULL);
	Color result;
	result.r = (uint8_t)clamp((float)(this->r * rhs.r), 0.0f, 255.0f);
	result.g = (uint8_t)clamp((float)(this->g * rhs.g), 0.0f, 255.0f);
	result.b = (uint8_t)clamp((float)(this->b * rhs.b), 0.0f, 255.0f);
	return result;
};
Color Color::operator /(const float rhs) {
	assert(this != NULL);
	Color result;
	result.r = (uint8_t)clamp(this->r / rhs, 0, 255);
	result.g = (uint8_t)clamp(this->g / rhs, 0, 255);
	result.b = (uint8_t)clamp(this->b / rhs, 0, 255);
	return result;
};
