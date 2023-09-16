#pragma once
#ifndef IMAGEBUFFER_H
#define IMAGEBUFFER_H
#include "Color.h"
#include "Vector.h"

class ImageBuffer
{
public:

	const float ScreenSizeX = 240;
	const float ScreenSizeY = 135;
	gfxVector2 size;
	Color** buffer;

	ImageBuffer(ImageBuffer &rhs);
	ImageBuffer();
	~ImageBuffer();

	ImageBuffer& operator =(const ImageBuffer& rhs)&;
	ImageBuffer operator +(const ImageBuffer& rhs);
	ImageBuffer operator -(const ImageBuffer& rhs);
	//ImageBuffer operator *(const float rhs);


private:
};
#endif