#include "ImageBuffer.h"

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
    Color tempEmpty(0.0f, 0.0f, 0.0f, 0.0f);
    for (int i = 0; i < size.x; ++i)
    {
        for (int j = 0; j < size.y; ++j)
        {
            buffer[i][j] = tempEmpty;
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
    Color tempEmpty(0.0f, 0.0f, 0.0f, 0.0f);
    for (int i = 0; i < size.x; ++i)
    {
        for (int j = 0; j < size.y; ++j)
        {
            buffer[i][j] = tempEmpty;
        }
    }
    size.x = BufferSizeX;
    size.y = BufferSizeY;
    return;
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
    Color black;
    black.r = 0;
    black.g = 0;
    black.b = 0;
    black.a = 255;
    for (int i = 0; i < size.x; ++i)
    {
        for (int j = 0; j < size.y; ++j)
        {
            buffer[i][j] = black;
        }
    }
    return *this;
}

ImageBuffer& ImageBuffer::AddSprite(ImageBuffer *sprite, int posX, int posY)
{
    int endPointX = 0;
    int endPointY = 0;
    int tempPosX = 0;
    int tempPosY = 0;
    if (posX + sprite->size.x > BufferSizeX)
    {
        endPointX += ((posX + sprite->size.x) - BufferSizeX);
    }
    if (posX < 0)
    {
        tempPosX -= posX;
    }
    if (posY + sprite->size.y > BufferSizeY)
    {
        endPointY += ((posY + sprite->size.y) - BufferSizeY);
    }
    if (posY < 0)
    {
        tempPosY -= posY;
    }

    for (int x = 0; x < sprite->size.x - endPointX - tempPosX; ++x)
    {
        for (int y = 0; y < sprite->size.y - endPointY - tempPosY; ++y)
        {
            if (sprite->buffer[x][y].a != 0)
            {
                buffer[x + posX + tempPosX][y + posY + tempPosY] = sprite->buffer[x][y];
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