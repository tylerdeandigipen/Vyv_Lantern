#pragma once
#ifndef IMAGEBUFFER_H
#define IMAGEBUFFER_H
#include "Color.h"
#include "Vector.h"
#include "Collision.h"

class ImageBuffer
{
public:

    int BufferSizeX = 240;
    int BufferSizeY = 135;

    float screenScale = 6;
    gfxVector2 size;
    gfxVector2 position;
    std::string name = "";
    
    int layer = 0;
    Color *buffer;
    AABB aabb;

    ImageBuffer(const char* filename);
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
