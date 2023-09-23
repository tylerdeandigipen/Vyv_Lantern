#include "ImageBuffer.h"

ImageBuffer::ImageBuffer(ImageBuffer& rhs)
{
    //allocate buffer
    buffer = new Color *[PixelScreenSizeX];
    for (int i = 0; i < PixelScreenSizeX; ++i)
    {
        buffer[i] = new Color[PixelScreenSizeY];
    }
    for (int i = 0; i < PixelScreenSizeX; ++i)
    {
        for (int j = 0; j < PixelScreenSizeY; ++j)
        {
            buffer[i][j] = rhs.buffer[i][j];
        }
    }
    size.x = PixelScreenSizeX;
    size.y = PixelScreenSizeY;
}

ImageBuffer::ImageBuffer()
{
    //allocate buffer
    buffer = new Color* [PixelScreenSizeX];
    for (int i = 0; i < PixelScreenSizeX; ++i)
    {
        buffer[i] = new Color[PixelScreenSizeY];
    }
    Color tempEmpty(0.0f, 0.0f, 0.0f, 0.0f);
    for (int i = 0; i < size.x; ++i)
    {
        for (int j = 0; j < size.y; ++j)
        {
            buffer[i][j] = tempEmpty;
        }
    }
	size.x = PixelScreenSizeX;
	size.y = PixelScreenSizeY;
	return;
}

ImageBuffer::~ImageBuffer()
{
    //Free each sub-array
    for (int i = 0; i < PixelScreenSizeX; ++i) 
    {
        delete[] buffer[i];
    }
    //Free the array of pointers
    delete[] buffer;
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