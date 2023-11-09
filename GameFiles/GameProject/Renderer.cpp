//------------------------------------------------------------------------------
//
// File Name:	Renderer.cpp
// Author(s):	Tyler Dean, Thomas Stephenson
// Purpose:		The Renderer implementation
//
// Copyright � 2023 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------
#include "Renderer.h"
#include "Color.h"
#include "Light.h"
#include "Math_Header.h"
#include "Math.h"
#include "Vector.h"
#include "ImageBuffer.h"
#include "ScopeTimer.h"
#include "Engine.h"

#include <windows.h>
#include <SDL/SDL.h>
#include "imgui_impl_opengl3.h"
#include <glad/glad.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <string>
#include <assert.h>
#include <omp.h>

#define OneOver255 (1.0f / 255.0f)

Renderer* Renderer::instance = new Renderer;

Renderer* Renderer::GetInstance()
{
    return Renderer::instance;
}

Vector2 Renderer::GetCameraPosition(void)
{
	return CameraP;
}

void Renderer::SetCameraPosition(Vector2 NewCameraP)
{
	CameraP = NewCameraP;
}

void Renderer::RenderLightingPass()
{
    float lightMultiplier = 0;

    float IntensityR = 0.0f;
    float IntensityG = 0.0f;
    float IntensityB = 0.0f;

	int CameraOffsetX = (int)CameraP.x;
	int CameraOffsetY = (int)CameraP.y;

    inputBuffer->ClearImageBuffer();
    outputBuffer->ClearImageBuffer();

    lightBuffer->ClearImageBuffer();

    backgroundLayer->Blit(inputBuffer, -CameraOffsetX, -CameraOffsetY);
    objectLayer->Blit(inputBuffer);
    foregroundLayer->Blit(inputBuffer, -CameraOffsetX, -CameraOffsetY);
    particleManager->UpdateParticles();

    //for shadow casting
    foregroundLayer->Blit(lightBuffer, -CameraOffsetX, -CameraOffsetY);

    //becasue thomas's camera isnt acurate
    normalBufferPostCam->ClearImageBuffer();
    normalBuffer->Blit(normalBufferPostCam, -CameraOffsetX, -CameraOffsetY);

    RenderParticles();

#if 1
    const int xSize = (int)inputBuffer->size.x;
    const int ySize = (int)inputBuffer->size.y;
    #pragma omp parallel for collapse(3) private(lightMultiplier, IntensityR, IntensityG, IntensityB)
	for (int x = 0; x < xSize; ++x)
    {
        for (int y = 0; y < ySize; ++y)
        {
			IntensityR = 0.0f;
			IntensityG = 0.0f;
			IntensityB = 0.0f;

            for (int i = 0; i < numLights; ++i)
            {
                if (CalculateIfPixelIsLit(x, y, i) == true)
                {
                    Light* LightSource = lightSource + i;
                    float lightMultiplier = FindPixelLuminosity((float)x, (float)y, LightSource);

                    if (lightMultiplier != 0)
                    {
                        float R_F32 = ((float)LightSource->color.GetRed()) * OneOver255;
                        float G_F32 = ((float)LightSource->color.GetGreen()) * OneOver255;
                        float B_F32 = ((float)LightSource->color.GetBlue()) * OneOver255;
                        lightMultiplier *= LightSource->volumetricIntensity;

                        IntensityR += (lightMultiplier * R_F32);
                        IntensityG += (lightMultiplier * G_F32);
                        IntensityB += (lightMultiplier * B_F32);
                    }
                }
            }

            lightR[x][y] = IntensityR;
            lightG[x][y] = IntensityG;
            lightB[x][y] = IntensityB;
		}
    }
#else
	inputBuffer->Blit(outputBuffer);
#endif
}

int blurRangeHigh = 3;
int blurRangeLow = -2;
void Renderer::BlurLights()
{
    if (doBlur == true)
    {
        const int xSize = (int)inputBuffer->size.x;
        const int ySize = (int)inputBuffer->size.y;

        #pragma omp parallel for collapse(2)    
        for (int x = 0; x < xSize; ++x)
        {
            for (int y = 0; y < ySize; ++y)
            {
                blurLightR[x][y] = lightR[x][y];
                blurLightG[x][y] = lightG[x][y];
                blurLightB[x][y] = lightB[x][y];
            }
        }

        int count;
        #pragma omp parallel for collapse(2) private(count)
        for (int x = 0; x < xSize; ++x)
        {
            for (int y = 0; y < ySize; ++y)
            {
                lightR[x][y] = 0.0f;
                lightG[x][y] = 0.0f;
                lightB[x][y] = 0.0f;

                count = 0;
                for (int w = blurRangeLow; w < blurRangeHigh; ++w)
                {
                    for (int h = blurRangeLow; h < blurRangeHigh; ++h)
                    {
                        if (x + w < SCREEN_SIZE_X && y + h < SCREEN_SIZE_Y && x + w >= 0 && y + h >= 0)
                        {
                            lightR[x][y] += blurLightR[x + w][y + h];
                            lightG[x][y] += blurLightG[x + w][y + h];
                            lightB[x][y] += blurLightB[x + w][y + h];
                            count += 1;
                        }
                    }
                }
                lightR[x][y] /= count;
                lightG[x][y] /= count;
                lightB[x][y] /= count;
            }
        }   
    }
}

float Renderer::FindPixelLuminosity(float x, float y, Light *LightSource)
{
    float normalMin = 1;
    Vector2 LightP = LightSource->position - CameraP;
    float Result = 0.0f;
   
    switch(LightSource->Type)
    {
        case LightSourceType_Point:
        {
            Vector2 D = Vector2(x, y) - LightP;
            float DistanceSq = Vector2::DotProduct(D, D);
            float distance = sqrt(DistanceSq);
            float s = distance / LightSource->radius;
            if (s >= 1.0)
                return 0.0;
            float s2 = s * s;
            Result = LightSource->intensity * pow((1 - s2), 2) / (1 + LightSource->radialFalloff * s);
        } break;

        case LightSourceType_Directional:
        {
            // @TODO: Check this against actual real-world spotlights.
            float DegreesToRadians = 0.01745329251994329577f;
            float AngleRadians = DegreesToRadians * -LightSource->angle;
            float MinAngleRadians = DegreesToRadians * -LightSource->minAngle;
            float MaxAngleRadians = DegreesToRadians * -LightSource->maxAngle;

            // @TODO: Precompute this?
            Vector2 FrustumLeftEdge = Vector2(-sinf(MinAngleRadians), cosf(MinAngleRadians));
            Vector2 FrustumRightEdge = Vector2(-sinf(MaxAngleRadians), cosf(MaxAngleRadians));
            float dRadius = Vector2::Length(FrustumRightEdge - FrustumLeftEdge);
            
            Vector2 EmissionDirection = Vector2(-sinf(AngleRadians), cosf(AngleRadians));
            Vector2 EmissionTangent = Vector2(-EmissionDirection.y, EmissionDirection.x);
            Vector2 ToPixel = Vector2(x, y) - LightP;

            float Dot = Vector2::DotProduct(ToPixel, EmissionDirection);
            if(Dot > 0.0f)
            {
                float DistanceSq = Vector2::DotProduct(ToPixel, ToPixel);
                float Radius = Dot * dRadius;
                float distance = sqrt(DistanceSq);
                float s = distance / LightSource->radius;
                if (s >= 1.0)
                    return 0.0;
                float s2 = s * s;
                float EmitterDistance = LightSource->intensity * pow((1 - s2), 2) / (1 + LightSource->radialFalloff * s);
                float FrustumDistance = (Radius - (fabsf(Vector2::DotProduct(EmissionTangent, ToPixel))));
                if(FrustumDistance < 0.0f)
                {
                    FrustumDistance = 0.0f;
                }
                if(FrustumDistance > Radius)
                {
                    FrustumDistance = Radius;
                }

                Result = EmitterDistance * (FrustumDistance * LightSource->frustumWeight);
            }
        } break;

		default:
		{
			assert(!"Encountered a light source of an unknown type.");
		} break;
    }	

    //Normal map calculations
    Vector2 pos = (Vector2(x, y));
    Vector2 distFromLight = LightP - pos;
    Vector2 distNormalized;
    float normalR = (float)normalBufferPostCam->SampleColor(x, y).r;
    float normalG = (float)normalBufferPostCam->SampleColor(x, y).g;
    Vector2 normalDir = Vector2{ normalR / 255.0f, normalG / 255.0f };
    normalDir *= 2;
    normalDir -= Vector2{ 1,1 };
    distNormalized.Normalize(distNormalized, distFromLight);
    float normalFalloff = -Vector2::DotProduct(distNormalized, normalDir);
    normalFalloff += normalMin;
    normalFalloff = clamp(normalFalloff, 0.0f, 1.0f);
    Result = normalFalloff * Result;


    //float const LIGHTING_STEP_SIZE = 0.25f;
	//Result = LIGHTING_STEP_SIZE * floorf(Result / LIGHTING_STEP_SIZE);

    return(Result);
}

void Renderer::CalculateShadows()
{
    const int xSize = (int)inputBuffer->size.x;
    const int ySize = (int)inputBuffer->size.y;
    Color black{ 0,0,0,255 };
    const Vector2 cameraPos = CameraP;
    Vector2 lightPos;
    int shadowsCast = 0;

    #pragma omp parallel
    {
        #pragma omp for nowait collapse(3) private(lightPos, shadowsCast)
        for (int x = 0; x < xSize; ++x)
        {
            for (int y = 0; y < ySize; ++y)
            {
                if (lightR[x][y] != 0 && lightG[x][y] != 0 && lightB[x][y] != 0)
                {
                    for (int i = 0; i < numLights; ++i)
                    {
                        if (lightSource[i].intensity != 0)
                        {
                            lightPos = lightSource[i].position;

                            if (CheckLineForObject(lightPos.x - cameraPos.x, lightPos.y - cameraPos.y, x, y) == true)
                            {
                                shadowsCast += 1;
                                //remove break later when trying to do multiple shadow casters
                            }
                            else if (distanceSquared(lightPos.x - cameraPos.x, lightPos.y - cameraPos.y, x, y) <= lightSource[i].radius * lightSource[i].radius)
                            {
                                shadowsCast -= 1;
                            }
                        }
                    }
                    if (lightBuffer->SampleColor(x, y).GetAlpha() != 0)
                    {
                        //maybe here do ham algo with the tilemap instead of pixels and if any tiles inbetween player and target tile then dont sub?
                        shadowsCast -= 1;
                    }

                    if (shadowsCast >= 1)
                    {
                        lightR[x][y] = 0;
                        lightG[x][y] = 0;
                        lightB[x][y] = 0;
                    }
                    shadowsCast = 0;
                }
            }
        }
    }
    
}

bool Renderer::CalculateIfPixelIsLit(int x,int y, int i)
{
    const Vector2 cameraPos = CameraP;
    int diameter = 0;
    Vector2 lightPos;
    bool isLit = true;
    if (lightSource[i].intensity != 0)
    {
        diameter = lightSource[i].radius * lightSource[i].radius;
        lightPos = lightSource[i].position;

        if (distanceSquared(x, y, lightPos.x - cameraPos.x, lightPos.y - cameraPos.y) <= diameter)
        {
            if (CheckLineForObject(lightPos.x - cameraPos.x, lightPos.y - cameraPos.y, x, y) == true)
            {
                isLit = false;
            }
            else
            {
                isLit = true;
            }
        }
        else
        {
            isLit = false;
        }
    }

    if (isLit == false && lightBuffer->SampleColor(x, y).GetAlpha() != 0)
    {
        //maybe here do ham algo with the tilemap instead of pixels and if any tiles inbetween player and target tile then dont sub?
        isLit = true;
    }

    return isLit;
}

void Renderer::RenderParticles()
{
    Vector2 tempPos;
    for (int i = 0; i < particleManager->totalParticles; i++)
    {
        if (particleManager->particleArray[i] != NULL)
        {
            tempPos = particleManager->particleArray[i]->position;
            tempPos -= CameraP;
            if (tempPos.x >= 0 && tempPos.x < inputBuffer->BufferSizeX && tempPos.y >= 0 && tempPos.y < inputBuffer->BufferSizeY)
            {
                Color& DestPixel = inputBuffer->SampleColor(tempPos.x, tempPos.y);
                DestPixel = particleManager->particleArray[i]->color;
            }
        }
    }
}

#define TILE_SIZE 8
#define NUM_WALL_SPRITES 17
const int wallSpriteIndexes[NUM_WALL_SPRITES] = { 1, 2, 4, 5, 6, 7, 8, 9, 10, 11, 26, 36, 27, 28, 34, 35, 12 };
void Renderer::MakeTileMap(int** tileMapArray)
{
    foregroundLayer->ClearImageBuffer();
    backgroundLayer->ClearImageBuffer();
    ImageBuffer* clearTile = new ImageBuffer{8,8};

    for (int x = 0; x < tileMapSize.x; ++x)
    {
        for (int y = 0; y < tileMapSize.y; ++y)
        {
            if (tileSet[tileMapArray[x][y]])
            {
                bool isWall = false;
                for (int i = 0; i < NUM_WALL_SPRITES; i++)
                {
                    if (tileMapArray[x][y] == wallSpriteIndexes[i])
                    {
                        isWall = true;
                        break;
                    }
                }
                if (isWall == false)
                {
                    tileSet[tileMapArray[x][y]]->position = { (float)(x * TILE_SIZE), (float)(y * TILE_SIZE) };
                    backgroundLayer->AddSprite(tileSet[tileMapArray[x][y]]);

                    clearTile->position = { (float)(x * TILE_SIZE), (float)(y * TILE_SIZE) };
                    foregroundLayer->AddSprite(clearTile);
                }
                else
                {
                    tileSet[tileMapArray[x][y]]->position = { (float)(x * TILE_SIZE), (float)(y * TILE_SIZE) };
                    foregroundLayer->AddSprite(tileSet[tileMapArray[x][y]]);

                    clearTile->position = { (float)(x * TILE_SIZE), (float)(y * TILE_SIZE) };
                    backgroundLayer->AddSprite(clearTile);
                }

                if (normalTileSet[tileMapArray[x][y]])
                {
                    normalTileSet[tileMapArray[x][y]]->position = { (float)(x * TILE_SIZE), (float)(y * TILE_SIZE) };
                    normalBuffer->AddSprite(normalTileSet[tileMapArray[x][y]]);
                }
            }
            
        }
    }
}

void Renderer::AddTileToTileset(ImageBuffer* tile)
{
    tileSet[numTiles] = tile;
    numTiles += 1;
}

void Renderer::AddNormalToNormalTileset(ImageBuffer* tile)
{
    normalTileSet[numNormalTiles] = tile;
    numNormalTiles += 1;
}
// 0 = forward, 1 = down, 2 = up, 3 = blink
void Renderer::UpdateFace(int& faceState_)
{
    if (faceIndex == -1)
    {
        faceIndex = numAnimatedObjects;
        ImageBuffer* temp = CreateAnimatedObject("./Assets/PPM/Man_Faces.ppm", { 8,8 });
        temp->isCulled = true;
    }
    if (faceState >= 0 && faceState <= animatedObjects[1][0]->totalFrames && faceState == faceState_)
    {
        faceState = faceState_;
        if (animatedObjects[0][animatedObjects[0][0]->currentFrame]->isFlipped != animatedObjects[faceIndex][faceState]->isFlipped)
        {
            animatedObjects[faceIndex][faceState]->FlipSprite();
        }
        animatedObjects[0][animatedObjects[0][0]->currentFrame]->AddSprite(animatedObjects[faceIndex][faceState]);
    }
}

ImageBuffer* Renderer::CreateAnimatedObject(const std::string filename, Vector2 frameSize)  
{
	ImageBuffer *Result = NULL;
    ImageBuffer* spriteSheet = new ImageBuffer{ filename };
    if(spriteSheet->buffer && (frameSize.x > 0) && (frameSize.y > 0))
	{
		spriteSheet->position.y = 0;
		ImageBuffer* temp;

		for (int i = 0; i < spriteSheet->BufferSizeX / frameSize.x; i++)
		{
			temp = new ImageBuffer{ frameSize.x, frameSize.y };
			spriteSheet->position.x = -(frameSize.x * i);
			animatedObjects[numAnimatedObjects][i] = &temp->AddSprite(spriteSheet);
		}

        animatedObjects[numAnimatedObjects][0]->totalFrames = (spriteSheet->BufferSizeX / frameSize.x) - 1;
		numAnimatedObjects += 1;

        Result = animatedObjects[numAnimatedObjects - 1][0];
	}

    delete spriteSheet;
	return(Result);
}

Renderer::Renderer()
{
    outputBuffer = new ImageBuffer{ SCREEN_SIZE_X ,SCREEN_SIZE_Y };
    inputBuffer = new ImageBuffer{ SCREEN_SIZE_X ,SCREEN_SIZE_Y };
    objectLayer = new ImageBuffer{ SCREEN_SIZE_X ,SCREEN_SIZE_Y };
    backgroundLayer = new ImageBuffer{ SCREEN_SIZE_X ,SCREEN_SIZE_Y };
    foregroundLayer = new ImageBuffer{ SCREEN_SIZE_X ,SCREEN_SIZE_Y };
    normalBuffer = new ImageBuffer{ SCREEN_SIZE_X ,SCREEN_SIZE_Y };
    normalBufferPostCam = new ImageBuffer{ SCREEN_SIZE_X ,SCREEN_SIZE_Y };
    lightBuffer = new ImageBuffer{ SCREEN_SIZE_X ,SCREEN_SIZE_Y };

    particleManager = new ParticleManager;
    faceIndex = -1;
    faceState = NULL;
    outputBuffer->screenScale = screenScale;

    //i hate this fix later
    lightR = new float*[SCREEN_SIZE_X];
    lightG = new float*[SCREEN_SIZE_X];
    lightB = new float*[SCREEN_SIZE_X];

    for (int x = 0; x < SCREEN_SIZE_X; ++x) 
    {
        for (int y = 0; y < SCREEN_SIZE_Y; ++y) 
        {
            lightR[x] = new float[y];
            lightG[x] = new float[y];
            lightB[x] = new float[y];
        }
    }

    blurLightR = new float* [SCREEN_SIZE_X];
    blurLightG = new float* [SCREEN_SIZE_X];
    blurLightB = new float* [SCREEN_SIZE_X];

    for (int x = 0; x < SCREEN_SIZE_X; ++x)
    {
        for (int y = 0; y < SCREEN_SIZE_Y; ++y)
        {
            blurLightR[x] = new float[y];
            blurLightG[x] = new float[y];
            blurLightB[x] = new float[y];
        }
    }
    //end of hate

	DebugBuffer = new ImageBuffer(SCREEN_SIZE_X, SCREEN_SIZE_Y);

    startTime = SDL_GetTicks();
    PreviousFrameBeginTime = startTime;

}

void Renderer::CleanRenderer()
{
    outputBuffer->ClearImageBuffer();
    inputBuffer->ClearImageBuffer();
    objectLayer->ClearImageBuffer();
    backgroundLayer->ClearImageBuffer();
    foregroundLayer->ClearImageBuffer();
    normalBuffer->ClearImageBuffer();
    normalBufferPostCam->ClearImageBuffer();
    DebugBuffer->ClearImageBuffer();
    lightBuffer->ClearImageBuffer();
    particleManager->ClearParticles();
    if (menuBuffer != NULL)
    {
        menuBuffer->ClearImageBuffer();
    }
    ClearObjects();
    ClearTilesets();
    faceIndex = -1;
    numTiles = 0;
    numNormalTiles = 0;
    numObjects = 0;
    numAnimatedObjects = 0;
    numLights = 0;
    frameCount = 0;
    timer = 0;
    CameraP = Vector2{ 0,0 };
}

Renderer::~Renderer(void)
{
    delete outputBuffer;
    delete inputBuffer;
    delete objectLayer;
    delete backgroundLayer;
    delete foregroundLayer;
    delete normalBuffer;
    delete normalBufferPostCam;
    delete DebugBuffer;
    delete lightBuffer;
    delete particleManager;
    if (menuBuffer != NULL)
    {
        delete menuBuffer;
    }

    //Delete the lightR g and b and blur here later

    ClearObjects();
    ClearTilesets();

    glDeleteTextures(1, &OutputBufferTexture);
}

void Renderer::DrawLine(Vector2 P0, Vector2 P1, const Color &LineColor)
{
    P0 -= CameraP;
    P1 -= CameraP;
    
	int MinX = (int)min(P0.x, P1.x);
	int MinY = (int)min(P0.y, P1.y);
	int MaxX = (int)max(P0.x, P1.x);
	int MaxY = (int)max(P0.y, P1.y);

	int ClippedMinX = MinX;
	int ClippedMaxX = MaxX;
	int ClippedMinY = MinY;
	int ClippedMaxY = MaxY;
	if(ClippedMinX < 0)
	{
		ClippedMinX = 0;
	}
	if(ClippedMinY < 0)
	{
		ClippedMinY = 0;
	}
	if(ClippedMaxX > (DebugBuffer->BufferSizeX - 1))
	{
		ClippedMaxX = DebugBuffer->BufferSizeX - 1;
	}
	if(ClippedMaxY > (DebugBuffer->BufferSizeY - 1))
	{
		ClippedMaxY = DebugBuffer->BufferSizeY - 1;
	}

	Vector2 D = P1 - P0;
	Vector2 N = Vector2::Normalize(Vector2(-D.y, D.x));
	for(int PixelY = ClippedMinY; PixelY <= ClippedMaxY; ++PixelY)
	{
		for(int PixelX = ClippedMinX; PixelX <= ClippedMaxX; ++PixelX)
		{
			Vector2 PixelD = Vector2((float)PixelX + 0.5f, (float)PixelY + 0.5f) - P0;
            float d = fabsf(Vector2::DotProduct(N, PixelD));
			if(d <= 0.5f)
			{
				Color &DestPixel = DebugBuffer->SampleColor(PixelX, PixelY);
				DestPixel = LineColor;
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

void Renderer::DitherLights()
{
    if (isFullBright == true)
    {
        return;
    }

    float red;
    float green;
    float blue;

    float bayerNum;
    int corr;

    int i1;
    int i2;
    int i3;
    const int xSize = (int)inputBuffer->size.x;
    const int ySize = (int)inputBuffer->size.y;
    Color black{ 0,0,0,255 };

    #pragma omp parallel for collapse(2) shared(xSize, ySize) private(red, green, blue, bayerNum, corr, i1, i2, i3)
    for (int x = 0; x < xSize; ++x)
    {
        for (int y = 0; y < ySize; ++y)
        {
            Color& DestPixel = inputBuffer->SampleColor(x, y);
            Color scaledWhite{ 255,255,255,255 };
            if (inputBuffer->SampleColor(x, y) == black)
            {
                scaledWhite = black;
            }
            scaledWhite = scaledWhite.ScaleIndividual(lightR[x][y], lightG[x][y], lightB[x][y]);

            red = scaledWhite.GetRed();
            green = scaledWhite.GetGreen();
            blue = scaledWhite.GetBlue();

            bayerNum = BAYER_PATTERN_8X8[x % bayerSize][y % bayerSize];
            corr = (bayerNum / 7) - 2;	//	 -2 because: 256/7=36  36*7=252  256-252=4   4/2=2 - correction -2

            i1 = (blue + corr) / 36;
            i2 = (green + corr) / 36;
            i3 = (red + corr) / 36;

            clamp(i1, 0, 7);
            clamp(i2, 0, 7);
            clamp(i3, 0, 7);


            //force glowing eyes, maybe make an emisive mask later
            if (renderOnlyLights == false)
            {
                if (DestPixel == Color{ 196,215,165,255 })
                {
                    DestPixel = inputBuffer->SampleColor(x, y);
                }
                else
                {
                     DestPixel = DestPixel.ScaleIndividual(((float)i1 / 7), ((float)i1 / 7), ((float)i1 / 7));
                }
            }
            else
            {
                //DestPixel = Color{ (uint8_t)(i1 * 36), (uint8_t)(i2 * 36), (uint8_t)(i3 * 36), 255 };
                DestPixel = scaledWhite;

            }
        }
    }
    
}

void Renderer::RenderToOutbuffer()
{
    const int xSize = (int)inputBuffer->size.x;
    const int ySize = (int)inputBuffer->size.y;
    #pragma omp parallel for collapse(2)
    for (int x = 0; x < xSize; ++x)
    {
        for (int y = 0; y < ySize; ++y)
        {
            Color& DestPixel = outputBuffer->SampleColor(x, y);
            DestPixel = inputBuffer->SampleColor(x, y);

            //Debug conditions
            if (renderNormalMap == true)
            {
                DestPixel = lightBuffer->SampleColor(x, y);
            }
        }
    }
}

void Renderer::RenderMenu()
{
    if (menuBuffer == NULL)
    {
        MakeMenu("./Assets/PPM/Pause_Menu.ppm");
    }

    const int xSize = (int)inputBuffer->size.x;
    const int ySize = (int)inputBuffer->size.y;
    #pragma omp parallel for collapse(2)
    for (int x = 0; x < xSize; ++x)
    {
        for (int y = 0; y < ySize; ++y)
        {
            Color& DestPixel = outputBuffer->SampleColor(x, y);
            DestPixel = DestPixel / 2;
        }
    }

    menuBuffer->Blit(outputBuffer);

    //do pause stuff here
}

void Renderer::MakeMenu(const std::string filename)
{
    menuBuffer = new ImageBuffer{filename};
}

void Renderer::Update()
{
    Uint32 currentTime = SDL_GetTicks();
    ScopeTimer TestScopeTimer("Renderer::Update");

    RenderLightingPass();
    BlurLights();
    DitherLights();
    RenderToOutbuffer();

    if (Engine::GetInstance()->Paused() == true)
    {
        RenderMenu();
    }

    DebugBuffer->Blit(outputBuffer);
    DebugBuffer->ClearImageBuffer();


    float AverageFrameLength = 0.0f;
	for(uint32_t FrameIndex = 1; FrameIndex < _countof(PreviousFrameLengths); ++FrameIndex)
	{
        AverageFrameLength += PreviousFrameLengths[FrameIndex - 1];
        PreviousFrameLengths[FrameIndex] = PreviousFrameLengths[FrameIndex - 1];
	}

    float dtThisFrame = (float)(currentTime - PreviousFrameBeginTime) / 1000.0f;
    PreviousFrameLengths[0] = dtThisFrame;
    AverageFrameLength += dtThisFrame;
    AverageFrameLength /= (float)_countof(PreviousFrameLengths);
    
	float AverageFrameRate = 1.0f / AverageFrameLength;
	char WindowTextBuffer[128];
	sprintf_s(WindowTextBuffer, sizeof(WindowTextBuffer), "FPS: %.2f", AverageFrameRate);
    SDL_SetWindowTitle(window, WindowTextBuffer);
    

#ifdef _DEBUG
    /*
    for (int i = 0; i < numLights * 2; i++)
    {
        if (i % 2 == 0)
        {
            outputBuffer->buffer[i + 1 + (3 * outputBuffer->BufferSizeX)] = { 0,0,255,255 };
        }
    }
    */
#endif
    
    if(!OutputBufferTexture)
    {
        glGenTextures(1, &OutputBufferTexture);
    }

	// @TODO: Maybe rework image buffers to be alligned to GPU memory layouts
	assert(sizeof(Color) == sizeof(uint32_t));

    glClearColor(1.0f, 0.0f, 1.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    
    glEnable(GL_TEXTURE_2D);
    
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, OutputBufferTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8,
                 outputBuffer->BufferSizeX, outputBuffer->BufferSizeY,
                 0,
                 GL_RGBA, GL_UNSIGNED_BYTE,
                 (void *)outputBuffer->buffer);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    
    glBegin(GL_QUADS);
    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

    glTexCoord2f(0.0f, 0.0f);
    glVertex2f(-1.0f, 1.0f);

    glTexCoord2f(1.0f, 0.0f);
    glVertex2f(1.0f, 1.0f);

    glTexCoord2f(1.0f, 1.0f);
    glVertex2f(1.0f, -1.0f);

    glTexCoord2f(0.0f, 1.0f);
    glVertex2f(-1.0f, -1.0f);

    glEnd();

    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    
    SDL_GL_SwapWindow(window);
    
    objectLayer->ClearImageBuffer();
    PreviousFrameBeginTime = currentTime;
}

int Renderer::returnObjCnt()
{
    int countObjects = 0;
    for (int i = 0; i < MAX_OBJECTS; ++i) {
        if (objects[i] != nullptr) {
            countObjects++;
        }
    }
    return countObjects;
}

void Renderer::ClearObjects()
{
    for (int i = 0; i < MAX_OBJECTS; ++i) 
    {
        if (objects[i] != nullptr) 
        {
            delete objects[i];
        }
    }
}

void Renderer::ClearTilesets()
{
    for (int i = 0; i < 128; i++)
    {
        if (tileSet)
        {
            if (tileSet[i] != NULL)
            {
                delete tileSet[i];
                tileSet[i] = NULL;
            }
        }
        if (normalTileSet)
        {
            if (normalTileSet[i] != NULL)
            {
                delete normalTileSet[i];
                normalTileSet[i] = NULL;
            }
        }
    }
}

void Renderer::ResizeBuffers()
{
    delete outputBuffer;
    delete inputBuffer;
    delete objectLayer;
    delete backgroundLayer;
    delete foregroundLayer;

    outputBuffer = new ImageBuffer{tileMapSize.x * (TILE_SIZE + 1), tileMapSize.y * (TILE_SIZE + 1) };
    inputBuffer = new ImageBuffer{ tileMapSize.x * (TILE_SIZE + 1), tileMapSize.y * (TILE_SIZE + 1) };
    objectLayer = new ImageBuffer{ tileMapSize.x * (TILE_SIZE + 1), tileMapSize.y * (TILE_SIZE + 1) };
    backgroundLayer = new ImageBuffer{ tileMapSize.x * (TILE_SIZE + 1), tileMapSize.y * (TILE_SIZE + 1) };
    foregroundLayer = new ImageBuffer{ tileMapSize.x * (TILE_SIZE + 1), tileMapSize.y * (TILE_SIZE + 1) };
    outputBuffer->screenScale = screenScale;
}

bool Renderer::CheckLineForObject(int x1, int y1, int x2, int y2)
{
    int dx = abs(x2 - x1);
    int dy = abs(y2 - y1);
    int sx = (x1 < x2) ? 1 : -1;
    int sy = (y1 < y2) ? 1 : -1;

    int error = dx - dy;
    int x = x1;
    int y = y1;

    while (x != x2 || y != y2) 
    {
        if (lightBuffer->SampleColor(x, y).GetAlpha() != 0)
        {
           return true;
        }

        int error2 = 2 * error;

        if (error2 > -dy) {
            error -= dy;
            x += sx;
        }

        if (error2 < dx) {
            error += dx;
            y += sy;
        }
    }
    return false;
}

void Renderer::AddObject(ImageBuffer* sprite)
{
    if ((numObjects + 1) < MAX_OBJECTS)
    {
        objects[numObjects] = sprite;
        numObjects += 1;
    }
}

void Renderer::UpdateObjects()
{
    objectLayer->ClearImageBuffer();

    for(int i = 0; i < numObjects; ++i)
    {
        for(int l = 0; l < 3; ++l)
        {
            if (objects[i]->layer == l && objects[i]->totalFrames == 0 && objects[i]->isCulled == false)
            {
                objectLayer->AddSprite(objects[i], CameraP);
                break;
            }
        }
    }
    for (int i = 0; i < numAnimatedObjects; ++i)
    {
        for (int l = 0; l < 3; ++l)
        {
            if (animatedObjects[i][0]->layer == l && animatedObjects[i][0]->isCulled == false)
            {
                int frame = animatedObjects[i][0]->currentFrame;
                animatedObjects[i][frame]->position = animatedObjects[i][0]->position;
                if ((animatedObjects[i][0]->isFlipped == true && animatedObjects[i][frame]->isFlipped != true) || (animatedObjects[i][0]->isFlipped == false && animatedObjects[i][frame]->isFlipped != false))
                {
                    animatedObjects[i][frame]->FlipSprite();
                }
                objectLayer->AddSprite(animatedObjects[i][frame], CameraP);
                break;
            }
        }
    }
}

void Renderer::UpdateAnimations(float dt = 0)
{
    if (timer >= timeBetweenFrames)
    {
        for (int i = 0; i < numAnimatedObjects; ++i)
        {
            if (animatedObjects[i][0]->currentFrame == animatedObjects[i][0]->totalFrames)
            {
                animatedObjects[i][0]->currentFrame = 0;
            }
            else
            {
                animatedObjects[i][0]->currentFrame += 1;
            }
        }
        timer = 0;
    }
    else
    {
        timer += dt;
    }
}

ImageBuffer* Renderer::GetObjectByName(std::string name_)
{
    for (int i = 0; i < numObjects; ++i)
    {
        if (objects[i]->name == name_)
        {
            return objects[i];
        }
    }
    return NULL;
}

void Renderer::AddLight(Light light)
{
    if (numLights + 1 < MAX_LIGHT_SOURCES)
    {
		lightSource[numLights++] = light;
	}
}
