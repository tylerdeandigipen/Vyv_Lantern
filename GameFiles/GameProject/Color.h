#pragma once
#ifndef COLOR_H
#define COLOR_H

class Color
{
public:
	int r;
	int g;
	int b;
	int a;

	Color& operator+=(Color const& rhs)& {
		r += rhs.r;
		g += rhs.g;
		b += rhs.b;
		a += rhs.a;
		return *this;
	}
	Color operator+(const Color& rhs) {
		Color result;
		result.r = this->r + rhs.r;
		result.g = this->g + rhs.g;
		result.b = this->b + rhs.b;
		result.a = this->a + rhs.a;
		return result;
	}
	Color& operator-=(Color const& rhs)& {
		r -= rhs.r;
		g -= rhs.g;
		b -= rhs.b;
		a -= rhs.a;
		return *this;
	}
	Color operator-(const Color& rhs) {
		Color result;
		result.r = this->r - rhs.r;
		result.g = this->g - rhs.g;
		result.b = this->b - rhs.b;
		result.a = this->a - rhs.a;
		return result;
	}
	Color operator*(float rhs) {
		Color result;
		result.r = this->r * rhs;
		result.g = this->g * rhs;
		result.b = this->b * rhs;
		result.a = this->a * rhs;
		return result;
	}
	Color operator/(float rhs) {
		Color result;
		result.r = this->r / rhs;
		result.g = this->g / rhs;
		result.b = this->b / rhs;
		result.a = this->a / rhs;
		return result;
	}

private:
};


#endif 