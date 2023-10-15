#include "ImageBuffer.h"
#include <inttypes.h>

ImageBuffer::ImageBuffer(ImageBuffer& rhs)
{
    //allocate buffer
    buffer = new Color *[BufferSizeX];
    for (int i = 0; i < BufferSizeX; ++i)
    {
        buffer[i] = new Color[BufferSizeY];
    }
    for (int i = 0; i < BufferSizeX; ++i)
    {
        for (int j = 0; j < BufferSizeY; ++j)
        {
            buffer[i][j] = rhs.buffer[i][j];
        }
    }
    size.x = BufferSizeX;
    size.y = BufferSizeY;
}

ImageBuffer::ImageBuffer()
{
    //allocate buffer
    buffer = new Color* [BufferSizeX];
    for (int i = 0; i < BufferSizeX; ++i)
    {
        buffer[i] = new Color[BufferSizeY];
    }
    Color trans(0.0f, 0.0f, 0.0f, 0.0f);
    for (int i = 0; i < size.x; ++i)
    {
        for (int j = 0; j < size.y; ++j)
        {
            buffer[i][j] = trans;
        }
    }
	size.x = BufferSizeX;
	size.y = BufferSizeY;
	return;
}

ImageBuffer::ImageBuffer(float x, float y)
{
    //set size
    BufferSizeX = x;
    BufferSizeY = y;
    //allocate buffer
    buffer = new Color * [BufferSizeX];
    for (int i = 0; i < BufferSizeX; ++i)
    {
        buffer[i] = new Color[BufferSizeY];
    }
    Color trans(0.0f, 0.0f, 0.0f, 0.0f);
    for (int i = 0; i < size.x; ++i)
    {
        for (int j = 0; j < size.y; ++j)
        {
            buffer[i][j] = trans;
        }
    }
    size.x = BufferSizeX;
    size.y = BufferSizeY;
    return;
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

        buffer = new Color * [BufferSizeX];
        for (int i = 0; i < BufferSizeX; ++i)
        {
            buffer[i] = new Color[BufferSizeY];
        }
        Color trans(0.0f, 0.0f, 0.0f, 0.0f);
        size.x = BufferSizeX;
        size.y = BufferSizeY;
        
        for (int j = 0; j < BufferSizeY; ++j)
        {
            for (int i = 0; i < BufferSizeX; ++i)
            {
                fscanf_s(fp, "%" SCNu8, &red);
                fscanf_s(fp, "%" SCNu8, &green);
                fscanf_s(fp, "%" SCNu8, &blue);
                if (red == 0 && green == 0 && blue == 0)
                {
                    buffer[i][j] = trans;
                }
                else
                {
                    buffer[i][j] = { red, green, blue, 255 };
                }
            }
        }
    }
}

ImageBuffer::~ImageBuffer()
{
    //Free each sub-array
    for (int i = 0; i < BufferSizeX; ++i) 
    {
        delete[] buffer[i];
    }
    //Free the array of pointers
    delete[] buffer;
}
ImageBuffer& ImageBuffer::ClearImageBuffer()
{
    Color trans = { 0,0,0,0 };
    for (int i = 0; i < size.x; ++i)
    {
        for (int j = 0; j < size.y; ++j)
        {
            buffer[i][j] = trans;
        }
    }
    return *this;
}

void ImageBuffer::MergeLayers(ImageBuffer* bottom, ImageBuffer* top)
{
    for (int x = 0; x < size.x; ++x)
    {
        for (int y = 0; y < size.y; ++y)
        {
            if (top->buffer[x][y].GetAlpha() == 0)
            {
                buffer[x][y] = bottom->buffer[x][y];
            }
            else
            {
                buffer[x][y] = top->buffer[x][y];
            }
        }
    }
}

void ImageBuffer::MergeLayersIndvPixel(ImageBuffer* bottom, ImageBuffer* middle, ImageBuffer* top, int x, int y)
{
    if (top->buffer[x][y].GetAlpha() != 0)
    {
        buffer[x][y] = top->buffer[x][y];
    }
    else if (middle->buffer[x][y].GetAlpha() != 0)
    {
        buffer[x][y] = middle->buffer[x][y];
    }
    else if (bottom->buffer[x][y].GetAlpha() != 0)
    {
        buffer[x][y] = bottom->buffer[x][y];
    }
    else
        buffer[x][y] = { 0,0,0,0 };
}


ImageBuffer& ImageBuffer::AddSprite(ImageBuffer *sprite)
{
    int endPointX = 0;
    int endPointY = 0;
    int tempPosX = 0;
    int tempPosY = 0;
    if (sprite->position.x + sprite->size.x > BufferSizeX)
    {
        endPointX += ((sprite->position.x + sprite->size.x) - BufferSizeX);
    }
    if (sprite->position.x < 0)
    {
        tempPosX -= sprite->position.x;
    }
    if (sprite->position.y + sprite->size.y > BufferSizeY)
    {
        endPointY += ((sprite->position.y + sprite->size.y) - BufferSizeY);
    }
    if (sprite->position.y < 0)
    {
        tempPosY -= sprite->position.y;
    }

    for (int x = tempPosX; x < sprite->size.x - endPointX; ++x)
    {
        for (int y = tempPosY; y < sprite->size.y - endPointY; ++y)
        {
            if (sprite->buffer[x][y].GetAlpha() != 0)
            {
                buffer[x + (int)sprite->position.x][y + (int)sprite->position.y] = sprite->buffer[x][y];
            }
        }
    }
    return *this;
}

ImageBuffer& ImageBuffer::operator =(const ImageBuffer& rhs)&
{
    for (int i = 0; i < size.x; ++i)
    {
        for (int j = 0; j < size.y; ++j)
        {
            buffer[i][j] = rhs.buffer[i][j];
        }
    }
    return *this;
}
ImageBuffer ImageBuffer::operator +(const ImageBuffer& rhs)
{
    for (int i = 0; i < size.x; ++i)
    {
        for (int j = 0; j < size.y; ++j)
        {
            buffer[i][j] += rhs.buffer[i][j];
        }
    }
    return *this;
}
ImageBuffer ImageBuffer::operator -(const ImageBuffer& rhs)
{
    for (int i = 0; i < size.x; ++i)
    {
        for (int j = 0; j < size.y; ++j)
        {
            buffer[i][j] -= rhs.buffer[i][j];
        }
    }
    return *this;
}