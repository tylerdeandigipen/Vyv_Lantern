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
#include <iostream>

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

//inverse matrix, get determinant, flip the elements
/*
| a[0]		a[3]		a[6]	|
|								|
| a[1]		a[4]		a[7]	|
|								|
| a[2]		a[5]		a[8]	|
*/
gfxMatrix3 gfxMatrix3::getInverse() const
{
	gfxMatrix3 inv_matx(*this);
	//find determinant aqz + brz + cpy - ary - bpz - cqx
	float det = (a[0] * a[4] * a[8]) + (a[3] * a[7] * a[2]) + (a[6] * a[1] * a[5]) -
		(a[0] * a[7] * a[5]) - (a[3] * a[1] * a[8] - (a[6] * a[4] * a[2]));
	//if det is negative, throw error
	if (abs(det) < EPSILON)
	{
		throw ZeroDivision();
	}
	else
	{
		inv_matx.a[0] = a[4] * a[8] - a[5] * a[7];
		inv_matx.a[1] = -(a[3] * a[8] - a[5] * a[6]);
		inv_matx.a[2] = a[3] * a[7] - a[4] * a[6];
		inv_matx.a[3] = -(a[1] * a[8] - a[2] * a[7]);
		inv_matx.a[4] = a[0] * a[8] - a[2] * a[6];
		inv_matx.a[5] = -(a[0] * a[7] - a[1] * a[6]);
		inv_matx.a[6] = a[1] * a[5] - a[2] * a[4];
		inv_matx.a[7] = -(a[0] * a[5] - a[2] * a[3]);
		inv_matx.a[8] = a[0] * a[4] - a[1] * a[3];

		float inv_det = ((float)1.f) / det;

		inv_matx.a[0] *= inv_det;
		inv_matx.a[1] *= inv_det;
		inv_matx.a[2] *= inv_det;
		inv_matx.a[3] *= inv_det;
		inv_matx.a[4] *= inv_det;
		inv_matx.a[5] *= inv_det;
		inv_matx.a[6] *= inv_det;
		inv_matx.a[7] *= inv_det;
		inv_matx.a[8] *= inv_det;
	}
	return inv_matx;

}
/*
| cosO		-sinO		0		|
|								|
| sinO		cosO		0		|
|								|
| 0			0			1		|
*/
void gfxMatrix3::SetRotation(float degree, float x_Axis, float y_Axis)
{
	float rad = deg2rad(degree);
	gfxMatrix3 rot_mat;
	rot_mat.a[0] = cosf(rad) * x_Axis;
	rot_mat.a[1] = sinf(rad) * x_Axis;
	rot_mat.a[2] = 0;
	rot_mat.a[3] = -sinf(rad) * y_Axis;
	rot_mat.a[4] = cosf(rad) * y_Axis;
	rot_mat.a[5] = 0;
	rot_mat.a[6] = 0;
	rot_mat.a[7] = 0;
	rot_mat.a[8] = 1;


}

//set matrix to scaling matrix
/*
| a[sx]		a[3]		a[6]	|
|								|
| a[1]		a[sy]		a[7]	|
|								|
| a[2]		a[5]		a[8]	|
*/
void gfxMatrix3::SetScale(float sx, float sy)
{
	setIdentity();
	a[0] = sx;
	a[4] = sy;
}
//set matrix to translation matrix
/*
| a[0]		a[3]		a[tx]	|
|								|
| a[1]		a[4]		a[ty]	|
|								|
| a[2]		a[5]		a[8]	|
*/
void gfxMatrix3::SetTranslation(float tx, float ty)
{
	setIdentity();
	a[6] = tx;
	a[7] = ty;
}

gfxMatrix3 gfxMatrix3::operator*(const gfxMatrix3& rhs)
{
	gfxMatrix3 temp;
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			temp.a[i * 3 + j] = a[0 * 3 + j] * rhs.a[i * 3 + 0] + a[1 * 3 + j] * rhs.a[i * 3 + 1] + a[2 * 3 + j] * rhs.a[i * 3 + 2];
		}
	}
	return temp;
}

gfxVector2 gfxMatrix3::operator*(const gfxVector2& rhs)
{
	float b[3];

	for (int i = 0; i < 3; i++)
	{
		b[i] = (a[0 * 3 + i] * rhs.x) + (a[1 * 3 + i] * rhs.y) + (a[2 * 3 + i] * 1);
	}
	gfxVector2 temp(b[0], b[1]);
	return temp;
}

gfxMatrix3 gfxMatrix3::operator*(float value)
{
	gfxMatrix3 temp;
	for (int i = 0; i < 9; i++)
	{
		temp.a[i] = a[i] * value;
	}

	return temp;
}

gfxMatrix3& gfxMatrix3::operator*=(const gfxMatrix3& rhs)
{
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			this->a[i * 3 + j] = a[0 * 3 + j] * rhs.a[i * 3 + 0] + this->a[1 * 3 + j] * rhs.a[i * 3 + 1] + this->a[2 * 3 + j] * rhs.a[i * 3 + 2];
		}
	}
	return *this;
}

gfxMatrix3& gfxMatrix3::operator*=(float value)
{
	for (int i = 0; i < 9; i++)
	{
		this->a[i] = a[i] * value;
	}

	return *this;
}

gfxMatrix3 gfxMatrix3::operator+(const gfxMatrix3& rhs)
{
	return gfxMatrix3(
		a[0] + rhs.a[0],
		a[1] + rhs.a[1],
		a[2] + rhs.a[2],
		a[3] + rhs.a[3],
		a[4] + rhs.a[4],
		a[5] + rhs.a[5],
		a[6] + rhs.a[6],
		a[7] + rhs.a[7],
		a[8] + rhs.a[8]
	);
}

gfxMatrix3& gfxMatrix3::operator=(const gfxMatrix3& rhs)
{
	for (int i = 0; i < 9; i++)
	{
		a[i] = rhs.a[i];
	}
	return *this;
}

float gfxMatrix3::operator[](int elem) const
{
	return this->a[elem];
}

float& gfxMatrix3::operator[](int elem)
{
	return this->a[elem];
}