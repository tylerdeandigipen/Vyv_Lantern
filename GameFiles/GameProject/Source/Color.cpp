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
	return r;
}

uint8_t Color::GetGreen()
{
	return g;
}

uint8_t Color::GetBlue()
{
	return b;
}

uint8_t Color::GetAlpha()
{
	return a;
}

void Color::SetAlpha(uint8_t a_)
{
	a = a_;
}

Color Color::BlendColors(Color top, Color bottom, float blendPercent)
{
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

Color Color::ScaleIndividual(float rScale, float gScale, float bScale)
{
	Color result;
	result.r = (uint8_t)clamp((float)this->r * rScale, 0.0f, 255.0f);
	result.g = (uint8_t)clamp((float)this->g * gScale, 0.0f, 255.0f);
	result.b = (uint8_t)clamp((float)this->b * bScale, 0.0f, 255.0f);
	return result;
}
bool Color::operator ==(const Color& rhs)&
{
	if (r == rhs.r && g == rhs.g && b == rhs.b)
	{
		return true;
	}
	return false;
}
bool Color::operator !=(const Color& rhs)&
{
	if (r != rhs.r || g != rhs.g || b != rhs.b)
	{
		return true;
	}
	return false;
}

Color& Color::operator =(Color const& rhs)& {
	r = rhs.r;
	g = rhs.g;
	b = rhs.b;
	a = rhs.a;
	return *this;
};

Color& Color::operator +=(Color const& rhs)& {
	r = (uint8_t)clamp((float)(r + rhs.r), 0.0f, 255.0f);
	g = (uint8_t)clamp((float)(g + rhs.g), 0.0f, 255.0f);
	b = (uint8_t)clamp((float)(b + rhs.b), 0.0f, 255.0f);
	return *this;
};
Color Color::operator +(const Color& rhs) {
	Color result;
	result.r = (uint8_t)clamp((float)(this->r + rhs.r), 0.0f, 255.0f);
	result.g = (uint8_t)clamp((float)(this->g + rhs.g), 0.0f, 255.0f);
	result.b = (uint8_t)clamp((float)(this->b + rhs.b), 0.0f, 255.0f);

	return result;
};
Color& Color::operator -=(Color const& rhs)& {
	r = (uint8_t)clamp((float)(r - rhs.r), 0.0f, 255.0f);
	g = (uint8_t)clamp((float)(g - rhs.g), 0.0f, 255.0f);
	b = (uint8_t)clamp((float)(b - rhs.b), 0.0f, 255.0f);
	return *this;
};
Color Color::operator -(const Color& rhs) {
	Color result;
	result.r = (uint8_t)clamp((float)(this->r - rhs.r), 0.0f, 255.0f);
	result.g = (uint8_t)clamp((float)(this->g - rhs.g), 0.0f, 255.0f);
	result.b = (uint8_t)clamp((float)(this->b - rhs.b), 0.0f, 255.0f);
	return result;
};
Color Color::operator *(const float rhs) {
	Color result;
	result.r = (uint8_t)clamp((float)(this->r * rhs), 0.0f, 255.0f);
	result.g = (uint8_t)clamp((float)(this->g * rhs), 0.0f, 255.0f);
	result.b = (uint8_t)clamp((float)(this->b * rhs), 0.0f, 255.0f);
	return result;
};
Color Color::operator *(const Color& rhs) {
	Color result;
	result.r = (uint8_t)clamp((float)(this->r * rhs.r), 0.0f, 255.0f);
	result.g = (uint8_t)clamp((float)(this->g * rhs.g), 0.0f, 255.0f);
	result.b = (uint8_t)clamp((float)(this->b * rhs.b), 0.0f, 255.0f);
	return result;
};
Color Color::operator /(const float rhs) {
	Color result;
	result.r = (uint8_t)clamp(this->r / rhs, 0, 255);
	result.g = (uint8_t)clamp(this->g / rhs, 0, 255);
	result.b = (uint8_t)clamp(this->b / rhs, 0, 255);
	return result;
};
