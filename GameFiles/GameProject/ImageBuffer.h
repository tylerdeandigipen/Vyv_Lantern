#pragma once
#ifndef IMAGEBUFFER_H
#define IMAGEBUFFER_H
#include "Color.h"
#include "Vector.h"

class ImageBuffer
{
public:

	const float RenderScreenSizeX = 960;
	const float RenderScreenSizeY = 540;
	const float PixelScreenSizeX = 240;
	const float PixelScreenSizeY = 135;
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