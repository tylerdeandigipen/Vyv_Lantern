#include "Color.h"

float clamp(float value, float min, float max);

Color::Color(float r, float g, float b, float a) // constructor definition
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

void Color::ColorClamp()
{
	r = clamp(r,0,255);
	g = clamp(g, 0, 255);
	b = clamp(b, 0, 255);
	a = clamp(a, 0, 255);
}

float Color::GetAlpha()
{
	return a;
}

Color& Color::operator =(Color const& rhs)& {
	r = rhs.r;
	g = rhs.g;
	b = rhs.b;
	a = rhs.a;
	ColorClamp();
	return *this;
};

Color& Color::operator +=(Color const& rhs)& {
	r += rhs.r;
	g += rhs.g;
	b += rhs.b;
	a += rhs.a;
	ColorClamp();
	return *this;
};
Color Color::operator +(const Color& rhs) {
	Color result;
	result.r = this->r + rhs.r;
	result.g = this->g + rhs.g;
	result.b = this->b + rhs.b;
	result.a = this->a + rhs.a;
	ColorClamp();
	return result;
};
Color& Color::operator -=(Color const& rhs)& {
	r -= rhs.r;
	g -= rhs.g;
	b -= rhs.b;
	a -= rhs.a;
	ColorClamp();
	return *this;
};
Color Color::operator -(const Color& rhs) {
	Color result;
	result.r = this->r - rhs.r;
	result.g = this->g - rhs.g;
	result.b = this->b - rhs.b;
	result.a = this->a - rhs.a;
	ColorClamp();
	return result;
};
Color Color::operator *(const float rhs) {
	Color result;
	result.r = this->r * rhs;
	result.g = this->g * rhs;
	result.b = this->b * rhs;
	result.a = this->a * rhs;
	ColorClamp();
	return result;
};
Color Color::operator *(const Color& rhs) {
	Color result;
	result.r = this->r * rhs.r;
	result.g = this->g * rhs.g;
	result.b = this->b * rhs.b;
	result.a = this->a * rhs.a;
	ColorClamp();
	return result;
};
Color Color::operator /(const float rhs) {
	Color result;
	result.r = this->r / rhs;
	result.g = this->g / rhs;
	result.b = this->b / rhs;
	result.a = this->a / rhs;
	ColorClamp();
	return result;
};