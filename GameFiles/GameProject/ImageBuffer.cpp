#include "ImageBuffer.h"
#include <inttypes.h>
#include <omp.h>

inline int min(int x, int y)
{
	int Result = x;
	if(y < x)
	{
		Result = y;
	}

	return(Result);
}

inline int max(int x, int y)
{
	int Result = x;
	if(y > x)
	{
		Result = y;
	}

	return(Result);
}

ImageBuffer::ImageBuffer(ImageBuffer& rhs)
{
    buffer = new Color [rhs.BufferSizeX*rhs.BufferSizeY];
    memcpy(buffer, rhs.buffer, rhs.BufferSizeX*rhs.BufferSizeY*sizeof(Color));

    BufferSizeX = rhs.BufferSizeX;
    BufferSizeY = rhs.BufferSizeY;
    
    size.x = rhs.BufferSizeX;
    size.y = rhs.BufferSizeY;
    isCulled = rhs.isCulled;

}

ImageBuffer::ImageBuffer()
{
    buffer = new Color[BufferSizeX * BufferSizeY];
    ClearImageBuffer();

	size.x = BufferSizeX;
	size.y = BufferSizeY;
    bool isCulled = false;
}

ImageBuffer::ImageBuffer(float x, float y)
{
	BufferSizeX = x;
	BufferSizeY = y;

	buffer = new Color[BufferSizeX * BufferSizeY];
    ClearImageBuffer();
    
	size.x = x;
	size.y = y;
    bool isCulled = false;
}

ImageBuffer::ImageBuffer(const std::string file)
{
    FILE* fp;
    const char* filename = file.c_str();
    fopen_s(&fp, filename, "r");
    if (fp)
    {
        int temp;
        char hold;
        uint8_t red = 0.0f;
        uint8_t green = 0.0f;
        uint8_t blue = 0.0f;
        
        fscanf_s(fp, "%c", &hold);
        fscanf_s(fp, "%c", &hold);
        fscanf_s(fp, "%d", &temp);
        BufferSizeX = temp;
        fscanf_s(fp, "%d", &temp);
        BufferSizeY = temp;
        fscanf_s(fp, "%d", &temp);

        buffer = new Color[BufferSizeX*BufferSizeY];

		Color trans(0.0f, 0.0f, 0.0f, 0.0f);
		size.x = BufferSizeX;
        size.y = BufferSizeY;
        
        for (int j = 0; j < BufferSizeY; ++j)
        {
			Color *DestPixel = buffer + (BufferSizeX * j);
            for (int i = 0; i < BufferSizeX; ++i)
            {
                fscanf_s(fp, "%" SCNu8, &red);
                fscanf_s(fp, "%" SCNu8, &green);
                fscanf_s(fp, "%" SCNu8, &blue);
                if (red == 0 && green == 0 && blue == 0)
                {
                    *DestPixel++ = trans;
                }
                else
                {
                    *DestPixel++ = { red, green, blue, 255 };
                }
            }
        }
    }
    bool isCulled = false;
}

ImageBuffer::~ImageBuffer()
{
    delete[] buffer;
}

ImageBuffer& ImageBuffer::ClearImageBuffer()
{
    Color trans = { 0,0,0,0 };

	Color *DestBuffer = buffer;
    
	for(int PixelIndex = 0; PixelIndex < (BufferSizeX * BufferSizeY); ++PixelIndex)
	{
		*DestBuffer++ = trans;
	}

	return *this;
}

void ImageBuffer::MergeLayers(ImageBuffer* bottom, ImageBuffer* top)
{
    for (int y = 0; y < size.y; ++y)
    {
        Color *DestPixel = buffer + (y * BufferSizeX);
        for (int x = 0; x < size.x; ++x)
        {
            Color TopSample = top->buffer[(y * top->BufferSizeY) + x];
            Color BottomSample = bottom->buffer[(y * bottom->BufferSizeY) + x];
            
            *DestPixel++ = (TopSample.GetAlpha()) ? TopSample : BottomSample;
        }
    }
}

void ImageBuffer::MergeLayersIndvPixel(ImageBuffer* bottom, ImageBuffer* middle, ImageBuffer* top, int x, int y)
{
    Color &DestSample = SampleColor(x, y);
    Color &BottomSample = bottom->SampleColor(x, y);
    Color &MiddleSample = middle->SampleColor(x, y);
    Color &TopSample = top->SampleColor(x, y);
    
    if (TopSample.GetAlpha() != 0)
    {
        DestSample = TopSample;
    }
    else if (MiddleSample.GetAlpha() != 0)
    {
        DestSample = MiddleSample;
    }
    else if (BottomSample.GetAlpha() != 0)
    {
        DestSample = BottomSample;
    }
    else
    {
        DestSample = { 0,0,0,0 };
    }
}


ImageBuffer& ImageBuffer::AddSprite(ImageBuffer *sprite, Vector2 CameraP)
{
	Vector2 ScreenSpaceP = sprite->position - CameraP;
    sprite->Blit(this, (int)ScreenSpaceP.x, (int)ScreenSpaceP.y);
    return *this;
}

void ImageBuffer::Blit(ImageBuffer *Destination, int OffsetX, int OffsetY)
{
    int MinX = max(OffsetX, 0);
    int MinY = max(OffsetY, 0);
    int MaxX = min(OffsetX + BufferSizeX, Destination->BufferSizeX);
    int MaxY = min(OffsetY + BufferSizeY, Destination->BufferSizeY);

	int SourceOffsetX = 0;
	int SourceOffsetY = 0;
	if(OffsetX < 0)
	{
		SourceOffsetX = -OffsetX;
	}
	if(OffsetY < 0)
	{
		SourceOffsetY = -OffsetY;
	}

    #pragma omp parallel for collapse(2)
    for(int y = MinY; y < MaxY; ++y)
    {
        Color *DestPixel = Destination->buffer + (y * Destination->BufferSizeX) + MinX;
        Color *SourcePixel = buffer + ((y - MinY + SourceOffsetY) * BufferSizeX) + SourceOffsetX;
        
        for(int x = MinX; x < MaxX; ++x)
        {
            if(SourcePixel->GetAlpha() != 0)
            {
                *DestPixel = *SourcePixel;
            }

            DestPixel++;
            SourcePixel++;
        }
    }
}

void ImageBuffer::FlipSprite()
{
    Color temp;
    int ySize = size.y;
    int xSize = size.x;
    for (int y = 0; y < size.y; ++y)
    {
        for (int x = 0; x < size.x / 2; ++x)
        {
            temp = buffer[(y * ySize) + x];
            buffer[(y * ySize) + x] = buffer[(y * ySize) + (xSize - x - 1)];
            buffer[(y * ySize) + (xSize - x - 1)] = temp;
        }
    }
    if (isFlipped == true)
    {
        isFlipped = false;
    }
    else
    {
        isFlipped = true;
    }
}

Color &ImageBuffer::SampleColor(int x, int y)
{
    Color &Result = buffer[x + (BufferSizeX * y)];
    return(Result);
}

ImageBuffer& ImageBuffer::operator =(const ImageBuffer& rhs)
{
    memcpy(buffer, rhs.buffer, BufferSizeX*BufferSizeY);
    return *this;
}
