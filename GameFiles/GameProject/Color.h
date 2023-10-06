#pragma once
#ifndef COLOR_H
#define COLOR_H

class Color
{
public:

	float r = 0;
	float g = 0;
	float b = 0;
	float a = 0;

	Color(float r, float g, float b, float a);
	Color(Color &rhs);
	Color();
	void ColorClamp();

	float GetAlpha();

	Color& operator =(const Color& rhs)&;
	Color& operator +=(const Color& rhs)&;
	Color operator +(const Color& rhs);
	Color& operator -=(const Color& rhs)&;
	Color operator -(const Color& rhs);
	Color operator *(const Color& rhs);
	Color operator *(const float rhs);
	Color operator /(const float rhs);

private:

};


#endif 