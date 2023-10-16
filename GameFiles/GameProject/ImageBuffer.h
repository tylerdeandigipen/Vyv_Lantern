#pragma once
#ifndef IMAGEBUFFER_H
#define IMAGEBUFFER_H
#include "Color.h"
#include "Vector.h"
#include "Collision.h"

enum ImageType
{
	/*Add what type of object is it*/
	COLLIDABLE,
	SWITCH,
	PLAYER
};

class ImageBuffer
{
	public:

		ImageType type;

		float BufferSizeX = 240;
		float BufferSizeY = 135;
		float screenScale = 6;
		gfxVector2 size;
		gfxVector2 position;
		std::string name = "";
		int layer = 0;
		Color** buffer;
		AABB aabb;

		ImageBuffer(const char* filename);
		ImageBuffer(ImageBuffer &rhs);
		ImageBuffer(float x, float y);
		ImageBuffer();
		~ImageBuffer();

		void Update(float dt);

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