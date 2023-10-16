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

    ImageBuffer(const std::string filename);
    ImageBuffer(ImageBuffer &rhs);
    ImageBuffer(float x, float y);
    ImageBuffer();
    ~ImageBuffer();

    void Blit(ImageBuffer *Destination, int OffsetX = 0, int OffsetY = 0);
    
    void Update(float dt);

    Color &SampleColor(int x, int y);
    
    ImageBuffer& ClearImageBuffer();
    void MergeLayers(ImageBuffer* bottom, ImageBuffer* top);
    void MergeLayersIndvPixel(ImageBuffer* bottom, ImageBuffer* middle, ImageBuffer* top, int x, int y);
    ImageBuffer& AddSprite(ImageBuffer *sprite);
    
    ImageBuffer& operator =(const ImageBuffer& rhs);
private:
};
#endif
