#include "ImageBuffer.h"
#include "Math.h"
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

    type = rhs.type;
    BufferSizeX = rhs.BufferSizeX;
    BufferSizeY = rhs.BufferSizeY;
    
    size.x = (float)rhs.BufferSizeX;
    size.y = (float)rhs.BufferSizeY;
    isCulled = rhs.isCulled;

}

ImageBuffer::ImageBuffer() : buffer(new Color[BufferSizeX * BufferSizeY])
{
    //buffer = new Color[BufferSizeX * BufferSizeY];
    ClearImageBuffer();

	size.x = (float)BufferSizeX;
	size.y = (float)BufferSizeY;
    bool isCulled = false;
}

ImageBuffer::ImageBuffer(float x, float y)
{
	BufferSizeX = static_cast<int>(x);
	BufferSizeY = static_cast<int>(y);

	buffer = new Color[BufferSizeX * BufferSizeY];
    ClearImageBuffer();
    
	size.x = x;
	size.y = y;
    bool isCulled = false;
}

ImageBuffer::ImageBuffer(const std::string file)
{
    buffer = NULL;
    FILE* fp;
    const char* filename = file.c_str();
    fopen_s(&fp, filename, "r");
    if (fp)
    {
        int temp;
        char hold;
        uint8_t red = 0;
        uint8_t green = 0;
        uint8_t blue = 0;
        
        fscanf_s(fp, "%c", &hold);
        fscanf_s(fp, "%c", &hold);
        fscanf_s(fp, "%d", &temp);
        BufferSizeX = temp;
        fscanf_s(fp, "%d", &temp);
        BufferSizeY = temp;
        fscanf_s(fp, "%d", &temp);

        buffer = new Color[BufferSizeX*BufferSizeY];

		Color trans((uint8_t)0, (uint8_t)0, (uint8_t)0, (uint8_t)0);
		size.x = (float)BufferSizeX;
        size.y = (float)BufferSizeY;
        
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
    type = NA;
    bool isCulled = false;
}

ImageBuffer::~ImageBuffer()
{
    if (buffer != NULL)
    {
        delete[] buffer;
        buffer = NULL;
    }
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
    int ySize = (int)size.y;
    int xSize = (int)size.x;
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

void ImageBuffer::BlurBuffer(int imageBlurRangelow, int imageBlurRangeHigh)
{
    ImageBuffer* tempBuffer = new ImageBuffer{*this};
    int count;
    int xSize = size.x;
    int ySize = size.y;
    #pragma omp parallel for collapse(4) private(count)
    for (int x = 0; x < xSize; ++x)
    {
        for (int y = 0; y < ySize; ++y)
        {
            Color& dest = buffer[x + (BufferSizeX * y)];
            if (dest.a != 0)
            {
                float r = 0;
                float g = 0;
                float b = 0;

                count = 0;
                for (int w = imageBlurRangelow; w < imageBlurRangeHigh; ++w)
                {
                    for (int h = imageBlurRangelow; h < imageBlurRangeHigh; ++h)
                    {
                        if (x + w < size.x && y + h < size.y && x + w >= 0 && y + h >= 0)
                        {
                            Color temp = tempBuffer->SampleColor(x + w, y + h);

                            r += temp.r;
                            g += temp.g;
                            b += temp.b;
                            count += 1;
                        }
                    }
                }
                r /= count;
                g /= count;
                b /= count;
                dest = Color{ (uint8_t)r,(uint8_t)g,(uint8_t)b,(uint8_t)255 };
            }
        }
    }
    
    delete tempBuffer;
    
}

void ImageBuffer::FloorBrightness(float floor)
{
    Color clear{ 0, 0, 0, 0 };
    const int xSize = size.x;
    const int ySize = size.y;
    #pragma omp parallel for collapse(2) shared(clear)
    for (int x = 0; x < xSize; ++x)
    {
        for (int y = 0; y < ySize; ++y)
        {
            Color& DestPixel = buffer[x + (BufferSizeX * y)];
            if ((DestPixel.r + DestPixel.g + DestPixel.b) <= (floor * 3))
            {
                DestPixel = clear;
            }
        }
    }
}

void ImageBuffer::GenerateVornoiPoints()
{
    for (int i = 0; i < MAX_NUM_VORNOI_POINTS; ++i)
    {
        vornoiPoints[i] = Vector2{ (float)(rand() % (int)(size.x)), (float)(rand() % (int)(size.y)) };
    }
}

void ImageBuffer::MakeVornoiNoiseBuffer(Vector2 camPos, float falloffModifier, float brightnessMultiplier, float minBrightness)
{
    const int xSize = size.x;
    const int ySize = size.y;
    #pragma omp parallel
    {
    #pragma omp for nowait collapse(3)
        for (int x = 0; x < xSize; ++x)
        {
            for (int y = 0; y < ySize; ++y)
            {
                float oldDist = 9999;
                for (int i = 0; i < MAX_NUM_VORNOI_POINTS; ++i)
                {
                    float tempDist = distanceSquared(vornoiPoints[i].x - camPos.x, vornoiPoints[i].y - camPos.y, x, y);
                    if (oldDist > tempDist)
                    {
                        oldDist = tempDist;
                    }
                }
                oldDist = clamp((100 - oldDist) + falloffModifier, 0, 255);

                Color& DestPixel = buffer[x + (BufferSizeX * y)];
                if (oldDist != 0)
                {
                    float finalColor = (float)((oldDist * brightnessMultiplier) + minBrightness);
                    finalColor = clamp(finalColor, 0, 255);
                    DestPixel = Color{ (uint8_t)finalColor, (uint8_t)finalColor, (uint8_t)finalColor, 255 };
                }
                else
                {
                    DestPixel = Color{ 0, 0, 0, 0 };
                }
            }
        }
    }
}


const	int BAYER_PATTERN_4X4[4][4] = {	//	4x4 Bayer Dithering Matrix. Color levels: 17
{	 15, 195,  60, 240	},
{	135,  75, 180, 120	},
{	 45, 225,  30, 210	},
{	165, 105, 150,  90	}
};

const	int BAYER_PATTERN_8X8[8][8] = {	//	8x8 Bayer Dithering Matrix. Color levels: 65
{	  0, 128,  32, 160,   8, 136,  40, 168	},
{	192,  64, 224,  96, 200,  72, 232, 104	},
{	 48, 176,  16, 144,  56, 184,  24, 152	},
{	240, 112, 208,  80, 248, 120, 216,  88	},
{	 12, 140,  44, 172,   4, 132,  36, 164	},
{	204,  76, 236, 108, 196,  68, 228, 100	},
{	 60, 188,  28, 156,  52, 180,  20, 148	},
{	252, 124, 220,  92, 244, 116, 212,  84	}
};
int bayerSize = 8;

void ImageBuffer::DitherBuffer(ImageBuffer* output, bool renderOnlyLights, bool isFullbright, float** lightR, float** lightG, float** lightB)
{
    if (output == NULL)
    {
        output = this;
    }
    float red;
    float green;
    float blue;

    int bayerNum;
    int corr;

    int i1;
    int i2;
    int i3;
    const int xSize = (int)size.x;
    const int ySize = (int)size.y;
    Color black{ 0,0,0,255 };
    #pragma omp parallel
    {
        #pragma omp for nowait collapse(2) private(red, green, blue, bayerNum, corr, i1, i2, i3)
        for (int x = 0; x < xSize; ++x)
        {
            for (int y = 0; y < ySize; ++y)
            {
                if (isFullbright != true)
                {
                    Color& SourcePixel = buffer[x + (BufferSizeX * y)];
                    Color tempColor = SourcePixel;
                    if (lightR != NULL && lightG != NULL && lightB != NULL)
                    {
                        Color white{ 255,255,255,255 };
                        tempColor = white.ScaleIndividual(lightR[x][y], lightG[x][y], lightB[x][y]);
                    }
                    red = tempColor.GetRed();
                    green = tempColor.GetGreen();
                    blue = tempColor.GetBlue();

                    bayerNum = BAYER_PATTERN_8X8[x % bayerSize][y % bayerSize];
                    corr = (int)((bayerNum / 7) - 2);	//	 -2 because: 256/7=36  36*7=252  256-252=4   4/2=2 - correction -2

                    i1 = (int)((blue + corr) / 36);
                    i2 = (int)((green + corr) / 36);
                    i3 = (int)((red + corr) / 36);

                    clamp((float)i1, 0, 7);
                    clamp((float)i2, 0, 7);
                    clamp((float)i3, 0, 7);


                    Color& DestPixel = output->SampleColor(x, y);
                    //force glowing eyes, maybe make an emisive mask later
                    if (renderOnlyLights == false)
                    {
                        if (DestPixel == Color{ 196,215,165,255 })
                        {
                            DestPixel = buffer[x + (BufferSizeX * y)];
                        }
                        else
                        {
                            DestPixel = SourcePixel.ScaleIndividual(((float)i1 / 7), ((float)i1 / 7), ((float)i1 / 7));
                        }
                    }
                    else
                    {
                        Color white{ 255,255,255,255 };
                        DestPixel = white.ScaleIndividual(((float)i1 / 7), ((float)i1 / 7), ((float)i1 / 7));
                    }
                }
            }
        }
    }

}