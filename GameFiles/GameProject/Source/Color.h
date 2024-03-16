//------------------------------------------------------------------------------
//
// File Name:	Color.h
// Author(s):	Tyler Dean
// Purpose:		Color class declaration for managing RGBA color values.
//
// Copyright © 2023 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------
#pragma once
#ifndef COLOR_H
#define COLOR_H
#include <cstdint>

class Color
{
public:

	Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a);
	Color(Color &rhs);
	Color();

	uint8_t GetAlpha();
	uint8_t GetRed();
	uint8_t GetBlue();
	uint8_t GetGreen();
	Color BlendColors(Color top, Color bottom, float blendPercent);
	void ChangeSaturation(double percent);
	void SetAlpha(uint8_t a_);
	Color ScaleIndividual(float rScale, float gScale, float bScale);
	Color& operator =(const Color& rhs)&;
	bool operator ==(const Color& rhs)&;
	bool operator !=(const Color& rhs)&;

	Color& operator +=(const Color& rhs)&;
	Color operator +(const Color& rhs);
	Color& operator -=(const Color& rhs)&;
	Color operator -(const Color& rhs);
	Color operator *(const Color& rhs);
	Color operator *(const float rhs);
	Color operator /(const float rhs);
	Color& operator /=(const float rhs);

	uint8_t r = 0;
	uint8_t g = 0;
	uint8_t b = 0;
	uint8_t a = 0;
};


#endif 
