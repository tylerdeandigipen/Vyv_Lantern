#include "ImageBuffer.h"

ImageBuffer::ImageBuffer(ImageBuffer& rhs)
{
    //allocate buffer
    buffer = new Color * [ScreenSizeX];
    for (int i = 0; i < ScreenSizeX; i++)
    {
        buffer[i] = new Color[ScreenSizeY];
    }
    for (int i = 0; i < ScreenSizeX; i++)
    {
        for (int j = 0; j < ScreenSizeY; j++)
        {
            buffer[i][j] = rhs.buffer[i][j];
        }
    }
    size.x = ScreenSizeX;
    size.y = ScreenSizeY;
}

ImageBuffer::ImageBuffer()
{
    //allocate buffer
    buffer = new Color* [ScreenSizeX];
    for (int i = 0; i < ScreenSizeX; i++)
    {
        buffer[i] = new Color[ScreenSizeY];
    }
    Color tempEmpty(0.0f, 0.0f, 0.0f, 0.0f);
    for (int i = 0; i < ScreenSizeX; i++)
    {
        for (int j = 0; j < ScreenSizeY; j++)
        {
            buffer[i][j] = tempEmpty;
        }
    }
	size.x = ScreenSizeX;
	size.y = ScreenSizeY;
	return;
}

ImageBuffer::~ImageBuffer()
{
    //Free each sub-array
    for (int i = 0; i < ScreenSizeX; ++i) 
    {
        delete[] buffer[i];
    }
    //Free the array of pointers
    delete[] buffer;
}