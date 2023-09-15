#include "Color.h"


Color::Color(int r, int g, int b, int a) // constructor definition
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

Color& Color::operator =(Color const& rhs)& {
	r = rhs.r;
	g = rhs.g;
	b = rhs.b;
	a = rhs.a;
	return *this;
};

Color& Color::operator +=(Color const& rhs)& {
	r += rhs.r;
	g += rhs.g;
	b += rhs.b;
	a += rhs.a;
	return *this;
};
Color Color::operator +(const Color& rhs) {
	Color result;
	result.r = this->r + rhs.r;
	result.g = this->g + rhs.g;
	result.b = this->b + rhs.b;
	result.a = this->a + rhs.a;
	return result;
};
Color& Color::operator -=(Color const& rhs)& {
	r -= rhs.r;
	g -= rhs.g;
	b -= rhs.b;
	a -= rhs.a;
	return *this;
};
Color Color::operator -(const Color& rhs) {
	Color result;
	result.r = this->r - rhs.r;
	result.g = this->g - rhs.g;
	result.b = this->b - rhs.b;
	result.a = this->a - rhs.a;
	return result;
};
Color Color::operator *(const float rhs) {
	Color result;
	result.r = this->r * rhs;
	result.g = this->g * rhs;
	result.b = this->b * rhs;
	result.a = this->a * rhs;
	return result;
};
Color Color::operator /(const float rhs) {
	Color result;
	result.r = this->r / rhs;
	result.g = this->g / rhs;
	result.b = this->b / rhs;
	result.a = this->a / rhs;
	return result;
};