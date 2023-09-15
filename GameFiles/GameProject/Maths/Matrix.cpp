/*!
@file    Matrix.cpp
@author	 Louis Wang

Matrix functions for all matrix calculations
*//*__________________________________________________________________________*/

/*
* 3x3 matrix is col major meaning
				| a[0][0]	a[0][1]		a[0][2]	|
				|								|
				| a[1][0]	a[1][1]		a[1][2]	|
				|								|
				| a[2][0]	a[2][1]		a[2][2]	|
--------------------------------------------------------------------
				| a[0]		a[3]		a[6]	|
				|								|
				| a[1]		a[4]		a[7]	|
				|								|
				| a[2]		a[5]		a[8]	|
*/

/*                                                                   includes
----------------------------------------------------------------------------- */

#include "Matrix.h"

/*  _________________________________________________________________________ */

/*constructor*/
gfxMatrix3::gfxMatrix3(float a00, float a10, float a20, float a01, float a11, float a21, float a02, float a12, float a22)
{
	a[0] = a00;
	a[1] = a10;
	a[2] = a20;
	a[3] = a01;
	a[4] = a11;
	a[5] = a21;
	a[6] = a02;
	a[7] = a12;
	a[8] = a22;
}

/*m[9] because 3x3 matrix has 9 grids*/
gfxMatrix3::gfxMatrix3(const float m[9])
{
	for (int i = 0; i < 9; i++)
	{
		a[i] = m[i];
	}
}

gfxMatrix3::gfxMatrix3(const gfxMatrix3& rhs)
{
	for (int i = 0; i < 9; i++)
	{
		a[i] = rhs.a[i];
	}
}

// set all elements to 0 by looping thru the elements
void gfxMatrix3::setZero()
{
	for (int i = 0; i < 9; i++)
	{
		a[i] = 0;
	}
}

void gfxMatrix3::setIdentity()
{
	// set all elements to 0 first
	setZero();
	// set [0][0],[1][1] and [2][2] to 1
	a[0] = a[4] = a[8] = 1;
}

//swap x and y elements
void gfxMatrix3::swap(float& x, float& y)
{
	/*float temp = x;
	x = y;
	y = temp;*/

	std::swap(x, y);
}

//transpose the matrix(flipping) ( Have to test to see if it works)
void gfxMatrix3::Transpose()
{
	for (int i = 0; i < 3; i++)
	{
		for (int j = i + 1; j < 3; j++)
		{
			swap(a[i * 3 + j], a[(j * 3 + i)]);
		}
	}
}

gfxMatrix3 gfxMatrix3::getTranspose(gfxMatrix3 m) const
{
	gfxMatrix3 mtx(m);
	mtx.Transpose();
	return mtx;
}