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

	
	Color(int r, int g, int b, int a);
	Color(Color &rhs);
	Color();
	
	Color& operator =(const Color& rhs)&;
	Color& operator +=(const Color& rhs)&;
	Color operator +(const Color& rhs);
	Color& operator -=(const Color& rhs)&;
	Color operator -(const Color& rhs);
	Color operator *(const float rhs);
	Color operator /(const float rhs);

private:
};


#endif 