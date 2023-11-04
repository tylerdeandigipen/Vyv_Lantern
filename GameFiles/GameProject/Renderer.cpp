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

#include <windows.h>
#include <SDL/SDL.h>
#include "imgui_impl_opengl3.h"
#include <glad/glad.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <string>
#include <assert.h>

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
    int x = 0;
    int y = 0;
    int i = 0;
    float lightMultiplier = 0;
    float rScale = 0;
    float gScale = 0;
    float bScale = 0;
    float IntensityR = 0.0f;
    float IntensityG = 0.0f;
    float IntensityB = 0.0f;
    Color trans{ 0,0,0,0 };
    float R_F32 = 0;
    float G_F32 = 0;
    float B_F32 = 0;
	int CameraOffsetX = (int)CameraP.x;
	int CameraOffsetY = (int)CameraP.y;

	inputBuffer->ClearImageBuffer();
	outputBuffer->ClearImageBuffer();
	
	backgroundLayer->Blit(inputBuffer, -CameraOffsetX, -CameraOffsetY);
	objectLayer->Blit(inputBuffer);
	foregroundLayer->Blit(inputBuffer, -CameraOffsetX, -CameraOffsetY);
    particleManager->UpdateParticles();
  
    //becasue thomas's camera isnt acurate
    normalBufferPostCam->ClearImageBuffer();
    normalBuffer->Blit(normalBufferPostCam, -CameraOffsetX, -CameraOffsetY);

#if 1
    RenderParticles();
    //parallel loop idk if working or not
    //#pragma omp parallel for
	for (x = 0; x < inputBuffer->size.x; ++x)
    {
        for (y = 0; y < inputBuffer->size.y; ++y)
        {
			IntensityR = 0.0f;
			IntensityG = 0.0f;
			IntensityB = 0.0f;

            for (i = 0; i < numLights; ++i)
            {
				Light *LightSource = lightSource + i;
                float lightMultiplier = FindPixelLuminosity((float)x, (float)y, LightSource);

				if (lightMultiplier != 0)
                {
                    float R_F32 = ((float)LightSource->color.GetRed())    * OneOver255;
                    float G_F32 = ((float)LightSource->color.GetGreen())  * OneOver255;
                    float B_F32 = ((float)LightSource->color.GetBlue())   * OneOver255;
                    lightMultiplier *= LightSource->volumetricIntensity;
                    
                    IntensityR += (lightMultiplier * R_F32);
                    IntensityG += (lightMultiplier * G_F32);
                    IntensityB += (lightMultiplier * B_F32);
				}
            }
            lightR[x][y] = IntensityR;
            lightG[x][y] = IntensityG;
            lightB[x][y] = IntensityB;

            /*
            Color& DestPixel = outputBuffer->SampleColor(x, y);
            //force glowing eyes, maybe make an emisive mask later
            if (inputBuffer->SampleColor(x, y) == Color{ 196,215,165,255 })
            {
                DestPixel = inputBuffer->SampleColor(x, y);
            }
            else
                DestPixel = inputBuffer->SampleColor(x, y).ScaleIndividual(IntensityR, IntensityG, IntensityB);
            if (isFullBright == true)
            {
                DestPixel = inputBuffer->SampleColor(x, y);
            }
            if (renderNormalMap == true)
            {
                DestPixel = normalBufferPostCam->SampleColor(x, y);
            }
            */
		}
    }
#else
	inputBuffer->Blit(outputBuffer);
#endif
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


    //float const LIGHTING_STEP_SIZE = 0.35f;
	//Result = LIGHTING_STEP_SIZE * floorf(Result / LIGHTING_STEP_SIZE);

    return(Result);
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
void Renderer::MakeTileMap(int** tileMapArray)
{
    foregroundLayer->ClearImageBuffer();
    backgroundLayer->ClearImageBuffer();
    for (int x = 0; x < tileMapSize.x; ++x)
    {
        for (int y = 0; y < tileMapSize.y; ++y)
        {
            if (tileSet[tileMapArray[x][y]])
            {
                if (tileSet[tileMapArray[x][y]]->layer == 0)
                {
                    tileSet[tileMapArray[x][y]]->position = { (float)(x * TILE_SIZE), (float)(y * TILE_SIZE) };
                    backgroundLayer->AddSprite(tileSet[tileMapArray[x][y]]);
                }
                else
                {
                    tileSet[tileMapArray[x][y]]->position = { (float)(x * TILE_SIZE), (float)(y * TILE_SIZE) };
                    foregroundLayer->AddSprite(tileSet[tileMapArray[x][y]]);
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
uint8_t lightPallet[16] = {0, 17, 34, 51, 68, 85, 102, 119, 136, 153, 170, 187, 204, 221, 238, 255};

void Renderer::DitherLights()
{
    if(isFullBright == true)
    {
        return;
    }
    int y = 0;
    int x = 0;
    for (x = 0; x < inputBuffer->size.x; ++x)
    {
        for (y = 0; y < inputBuffer->size.y; ++y)
        {
            Color& DestPixel = inputBuffer->SampleColor(x, y);
            Color scaledWhite{ 255,255,255,255 };
            scaledWhite = scaledWhite.ScaleIndividual(lightR[x][y], lightG[x][y], lightB[x][y]);

            float red = scaledWhite.GetRed();
            float green = scaledWhite.GetGreen();
            float blue = scaledWhite.GetBlue();
            float bayerNum = BAYER_PATTERN_4X4[x % 4][y % 4];
            //this method only returns max or min sadly need fix later
            uint8_t red8 = (uint8_t)(red > bayerNum ? 255 : 0);
            uint8_t green8 = (uint8_t)(green > bayerNum ? 255 : 0);
            uint8_t blue8 = (uint8_t)(blue > bayerNum ? 255 : 0);
      
            //force glowing eyes, maybe make an emisive mask later
            if (renderOnlyLights == false)
            {
                if (DestPixel == Color{ 196,215,165,255 })
                {
                    DestPixel = inputBuffer->SampleColor(x, y);
                }
                else
                {
                    DestPixel = DestPixel.ScaleIndividual((float)(red8 / 255), (float)(green8 / 255), (float)(blue8 / 255));
                }
            }
            else
            {
                DestPixel = Color{red8, green8, blue8, 255};
            }
        }
    }
}

void Renderer::RenderToOutbuffer()
{
    int y = 0;
    int x = 0;
    for (x = 0; x < inputBuffer->size.x; ++x)
    {
        for (y = 0; y < inputBuffer->size.y; ++y)
        {
            Color& DestPixel = outputBuffer->SampleColor(x, y);
            DestPixel = inputBuffer->SampleColor(x, y);

            //Debug conditions
            if (renderNormalMap == true)
            {
                DestPixel = normalBufferPostCam->SampleColor(x, y);
            }
        }
    }
}
void Renderer::Update()
{
    Uint32 currentTime = SDL_GetTicks();
    ScopeTimer TestScopeTimer("Renderer::Update");

    RenderLightingPass();
    DitherLights();
    RenderToOutbuffer();
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
    for (int i = 0; i < numLights * 2; i++)
    {
        if (i % 2 == 0)
        {
            outputBuffer->buffer[i + 1 + (3 * outputBuffer->BufferSizeX)] = { 0,0,255,255 };
        }
    }
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

void Renderer::brensenhamalgo(int x1, int y1, int x2, int y2)
{
    int dx = abs(x2 - x1);
    int dy = abs(y2 - y1);
    int sx = (x1 < x2) ? 1 : -1;
    int sy = (y1 < y2) ? 1 : -1;

    int error = dx - dy;
    int x = x1;
    int y = y1;

    while (x != x2 || y != y2) {
        // Draw the current point (x, y) here
        std::cout << "(" << x << ", " << y << ")" << std::endl;

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
    /*DO THE DRAWINGS HERE TO DRAW THE LINE*/
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
