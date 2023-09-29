#pragma once
#ifndef IMAGEBUFFER_H
#define IMAGEBUFFER_H
#include "Color.h"
#include "Vector.h"

class ImageBuffer
{
	public:

		float BufferSizeX = 240;
		float BufferSizeY = 135;
		float screenScale = 6;
		gfxVector2 size;
		gfxVector2 position;
		std::string name = "";
		int layer = 0;
		Color** buffer;

		ImageBuffer(ImageBuffer &rhs);
		ImageBuffer(float x, float y);
		ImageBuffer();
		~ImageBuffer();

		ImageBuffer& ClearImageBuffer();
		void MergeLayers(ImageBuffer* bottom, ImageBuffer* top);
		void MergeLayersIndvPixel(ImageBuffer* bottom, ImageBuffer* middle, ImageBuffer* top, int x, int y);
		ImageBuffer& AddSprite(ImageBuffer *sprite);

		ImageBuffer& operator =(const ImageBuffer& rhs)&;
		ImageBuffer operator +(const ImageBuffer& rhs);
		ImageBuffer operator -(const ImageBuffer& rhs);
		//ImageBuffer operator *(const float rhs);


	private:
};
#endif