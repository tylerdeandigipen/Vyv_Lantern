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
	PLAYER,
    WALL
};

class ImageBuffer
{
public:

    ImageType type;

    int BufferSizeX = 0;
    int BufferSizeY = 0;
    float screenScale = 6;
    gfxVector2 size;
    gfxVector2 position;
    std::string name = "";
    int layer = 0;
    int currentFrame = 0;
    int totalFrames = 0;
    bool isFlipped = false;
    bool isCulled = false;
    Color* buffer;
    AABB aabb;

    ImageBuffer(const std::string filename);
    ImageBuffer(ImageBuffer &rhs);
    ImageBuffer(float x, float y);
    ImageBuffer();
    ~ImageBuffer();

	ImageBuffer& AddSprite(ImageBuffer* sprite, Vector2 CameraP = Vector2(0, 0));
	void Blit(ImageBuffer *Destination, int OffsetX = 0, int OffsetY = 0);
    
    void Update(float dt);

    Color &SampleColor(int x, int y);
    
    void FlipSprite();
    ImageBuffer& ClearImageBuffer();
    void MergeLayers(ImageBuffer* bottom, ImageBuffer* top);
    void MergeLayersIndvPixel(ImageBuffer* bottom, ImageBuffer* middle, ImageBuffer* top, int x, int y);
    
    ImageBuffer& operator =(const ImageBuffer& rhs);
private:
};
#endif
