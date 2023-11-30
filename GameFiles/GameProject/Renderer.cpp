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
#include "Time.h"
#include "SceneSystem.h"
#include "Inputs.h"
#include "DebugNew.h"
#include "imgui_impl_opengl3.h"
#include "FrameRate.h"

#include <windows.h>
#include <SDL/SDL.h>
#include <glad/glad.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <string>
#include <assert.h>
#include <omp.h>

#include "PauseMenu.h"
#include <thread>

#define OneOver255 (1.0f / 255.0f)

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

Renderer* Renderer::instance = new Renderer();



void Renderer::Update(float dt)
{
    Uint32 currentTime = SDL_GetTicks();
    ScopeTimer TestScopeTimer("Renderer::Update");

    UpdateObjects(dt);
    RenderLightingPass();
    BlurLights(-1,2);
    inputBuffer->DitherBuffer(inputBuffer, renderOnlyLights, isFullbright,  lightR, lightG, lightB);
    RenderFog();
    RenderToOutbuffer();

    DebugBuffer->Blit(outputBuffer, -GetCameraPosition().x, -GetCameraPosition().y);
    DebugBuffer->ClearImageBuffer();

    float AverageFrameRate = FrameRate::CalculateAverageFrameRate(PreviousFrameLengths, _countof(PreviousFrameLengths), currentTime, PreviousFrameBeginTime);

    FrameRate::UpdateWindowTitle(window, AverageFrameRate);

    FrameRate::capFrame(dt);



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

    if (!OutputBufferTexture)
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
        outputBuffer->size.x, outputBuffer->size.y,
        0,
        GL_RGBA, GL_UNSIGNED_BYTE,
        (void*)outputBuffer->buffer);
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

#ifndef Rendering_Functions

void Renderer::RenderToOutbuffer()
{
    Color black{ 0,0,0,255 };
    const int xSize = (int)inputBuffer->size.x;
    const int ySize = (int)inputBuffer->size.y;
    #pragma omp parallel for collapse(2) shared(black)
    for (int x = 0; x < xSize; ++x)
    {
        for (int y = 0; y < ySize; ++y)
        {
            Color& DestPixel = outputBuffer->SampleColor(x, y);
            if (renderWallHitboxes != true && drawRawFog != true)
            {
                if (y % 2 == 0 && doScanLines == true)
                {
                    DestPixel = (inputBuffer->SampleColor(x, y) * scanLineOpacity);
                }
                else
                {
                    DestPixel = inputBuffer->SampleColor(x, y);
                }
            }
            if (doFog == true)
            {
                Color fog = fogBufferPostCam->SampleColor(x, y);
                if (fog != black && DestPixel != black)
                {
                    DestPixel = fog.BlendColors(DestPixel, fog, fogOpacity);
                }
            }
            if (drawRawFog == true)
            {
                DestPixel = fogBufferPostCam->SampleColor(x, y);
            }
            if (renderNormalMap == true)
            {
                DestPixel = normalBufferPostCam->SampleColor(x, y);
            }
        }
    }

    if (renderWallHitboxes == true)
    {
        RenderWallCollidersToDebugBuffer();
    }

    if (Engine::GetInstance()->Paused() == true)
    {
        RenderMenu();
    }
}

void Renderer::RenderLightingPass()
{
    int CameraOffsetX = (int)CameraP.x;
    int CameraOffsetY = (int)CameraP.y;

    inputBuffer->ClearImageBuffer();
    outputBuffer->ClearImageBuffer();

    lightBuffer->ClearImageBuffer();

    backgroundLayer->Blit(inputBuffer, -CameraOffsetX, -CameraOffsetY);
    objectLayer->Blit(inputBuffer, -CameraOffsetX, -CameraOffsetY);
    foregroundLayer->Blit(inputBuffer, -CameraOffsetX, -CameraOffsetY);
    particleManager->UpdateParticles();

    //for shadow casting
    shadowCasterBuffer->Blit(lightBuffer, -CameraOffsetX, -CameraOffsetY);

    //becasue thomas's camera isnt acurate
    normalBufferPostCam->ClearImageBuffer();
    normalBuffer->Blit(normalBufferPostCam, -CameraOffsetX, -CameraOffsetY);

    RenderParticles();

#if 1
    const int xSize = (int)inputBuffer->size.x;
    const int ySize = (int)inputBuffer->size.y;

    float lightMultiplier = 0;

    float IntensityR = 0.0f;
    float IntensityG = 0.0f;
    float IntensityB = 0.0f;
#pragma omp parallel
    {
#pragma omp for collapse(3) nowait private(lightMultiplier, IntensityR, IntensityG, IntensityB)
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
    }
#else
    inputBuffer->Blit(outputBuffer);
#endif
}

float Renderer::FindPixelLuminosity(float x, float y, Light* LightSource)
{
    float normalMin = 0.75f;
    Vector2 LightP = LightSource->position - CameraP;
    float Result = 0.0f;

    switch (LightSource->Type)
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
        Result = LightSource->intensity * (float)pow((1 - s2), 2) / (1 + LightSource->radialFalloff * s);
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
        if (Dot > 0.0f)
        {
            float DistanceSq = Vector2::DotProduct(ToPixel, ToPixel);
            float Radius = Dot * dRadius;
            float distance = sqrt(DistanceSq);
            float s = distance / LightSource->radius;
            if (s >= 1.0)
                return 0.0;
            float s2 = s * s;
            float EmitterDistance = LightSource->intensity * (float)pow((1 - s2), 2) / (1 + LightSource->radialFalloff * s);
            float FrustumDistance = (Radius - (fabsf(Vector2::DotProduct(EmissionTangent, ToPixel))));
            if (FrustumDistance < 0.0f)
            {
                FrustumDistance = 0.0f;
            }
            if (FrustumDistance > Radius)
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
    float normalR = (float)normalBufferPostCam->SampleColor((int)x, (int)y).r;
    float normalG = (float)normalBufferPostCam->SampleColor((int)x, (int)y).g;
    if (normalR == 0 && normalG == 0)
    {
        return Result;
    }
    Vector2 pos = (Vector2(x, y));
    Vector2 distFromLight = LightP - pos;
    Vector2 distNormalized;
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

    return Result;
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

                            if (CheckLineForObject((int)lightPos.x - (int)cameraPos.x, (int)lightPos.y - (int)cameraPos.y, (int)x, (int)y) == true)
                            {
                                shadowsCast += 1;
                                //remove break later when trying to do multiple shadow casters
                            }
                            else if (distanceSquared(lightPos.x - cameraPos.x, lightPos.y - cameraPos.y, (float)x, (float)y) <= lightSource[i].radius * lightSource[i].radius)
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

bool Renderer::CalculateIfPixelIsLit(int x, int y, int i)
{
    const Vector2 cameraPos = CameraP;
    float diameter = 0;
    Vector2 lightPos;
    bool isLit = true;
    if (lightSource[i].intensity != 0)
    {
        diameter = lightSource[i].radius * lightSource[i].radius;
        lightPos = lightSource[i].position;
        if (distanceSquared((float)x, (float)y, lightPos.x - cameraPos.x, lightPos.y - cameraPos.y) <= diameter)
        {
            int inOutCount = CheckLineForObject((int)(lightPos.x - cameraPos.x), (int)(lightPos.y - cameraPos.y), x, y);
            if (inOutCount > 0)
            {
                isLit = false;
                if (lightBuffer->SampleColor(x, y).GetAlpha() != 0 && inOutCount == 1)
                {
                    isLit = true;
                }
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

    return isLit;
}

void Renderer::RenderMenu()
{
    if (menuBuffer == NULL)
    {
        LoadMenuToBuffer("./Assets/PPM/Pause_Temp.ppm");
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

    PauseMenu::GetInstance()->HandleInput();

    menuBuffer->Blit(outputBuffer);

}

void Renderer::RenderParticles()
{
    Vector2 tempPos;
    particleManager->tileMapSize = Vector2{ (float)foregroundLayer->BufferSizeX, (float)foregroundLayer->BufferSizeY };

#pragma omp for private(tempPos)
    for (int i = 0; i < particleManager->totalParticles; i++)
    {
        if (particleManager->particleArray[i] != NULL)
        {
            tempPos = particleManager->particleArray[i]->position;
            tempPos -= CameraP;
            if (tempPos.x >= 0 && tempPos.x < inputBuffer->size.x && tempPos.y >= 0 && tempPos.y < inputBuffer->size.y)
            {
                Color& DestPixel = inputBuffer->SampleColor((int)tempPos.x, (int)tempPos.y);
                DestPixel = particleManager->particleArray[i]->color;
            }
        }
    }
}

void Renderer::RenderFog()
{
    if (doFog == true)
    {
        if (fogIsDirty == true)
        {
            delete fogBuffer;
            fogBuffer = new ImageBuffer{ "./Assets/PPM/Premade_Fog_2.ppm" };
            fogBuffer->BlurBuffer(-4, 5);
            fogIsDirty = false;
        }
        fogPos += fogMoveDir;
        fogPos += GetCameraPosition();
        if (fogPos.x > SCREEN_SIZE_X || fogPos.x < -SCREEN_SIZE_X)
        {
            fogPos.x = 0;
        }
        if (fogPos.y > SCREEN_SIZE_Y || fogPos.y < -SCREEN_SIZE_Y)
        {
            fogPos.y = 0;
        }
        fogBufferPostCam->ClearImageBuffer();

        fogBuffer->Blit(fogBufferPostCam, -fogPos.x, -fogPos.y);

        fogBuffer->Blit(fogBufferPostCam, -fogPos.x + SCREEN_SIZE_X, -fogPos.y);
        fogBuffer->Blit(fogBufferPostCam, -fogPos.x, -fogPos.y + SCREEN_SIZE_Y);
        fogBuffer->Blit(fogBufferPostCam, -fogPos.x - SCREEN_SIZE_X, -fogPos.y);
        fogBuffer->Blit(fogBufferPostCam, -fogPos.x, -fogPos.y - SCREEN_SIZE_Y);

        fogBuffer->Blit(fogBufferPostCam, -fogPos.x + SCREEN_SIZE_X, -fogPos.y + SCREEN_SIZE_Y);
        fogBuffer->Blit(fogBufferPostCam, -fogPos.x + SCREEN_SIZE_X, -fogPos.y - SCREEN_SIZE_Y);
        fogBuffer->Blit(fogBufferPostCam, -fogPos.x - SCREEN_SIZE_X, -fogPos.y + SCREEN_SIZE_Y);
        fogBuffer->Blit(fogBufferPostCam, -fogPos.x - SCREEN_SIZE_X, -fogPos.y - SCREEN_SIZE_Y);
        fogPos -= GetCameraPosition();
        fogBufferPostCam->DitherBuffer();
    }
}

#endif

#ifndef Animation_Functions

void Renderer::UpdateAnimations(float dt = 0)
{
    UpdateFace(faceState);
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

ImageBuffer* Renderer::CreateAnimatedObject(const std::string filename, Vector2 frameSize)
{
    ImageBuffer* Result = NULL;
    ImageBuffer* spriteSheet = new ImageBuffer{ filename };
    if (spriteSheet->buffer && (frameSize.x > 0) && (frameSize.y > 0))
    {
        spriteSheet->position.y = 0;
        ImageBuffer* temp = NULL;

        for (int i = 0; i < spriteSheet->size.x / frameSize.x; i++)
        {
            temp = new ImageBuffer{ frameSize.x, frameSize.y };
            spriteSheet->position.x = -(frameSize.x * i);
            animatedObjects[numAnimatedObjects][i] = &temp->AddSprite(spriteSheet);
        }

        animatedObjects[numAnimatedObjects][0]->totalFrames = (int)((spriteSheet->size.x / frameSize.x) - 1);
        numAnimatedObjects += 1;

        Result = animatedObjects[numAnimatedObjects - 1][0];
    }

    delete spriteSheet;
    return(Result);
}

void Renderer::UpdateFace(int& faceState_)
{
    if (faceIndex == -1 || faceIndex > MAX_ANIMATED_OBJECTS)
    {
        faceIndex = numAnimatedObjects;
        ImageBuffer* temp = CreateAnimatedObject("./Assets/PPM/Man_Faces.ppm", { 8,8 });
        temp->isCulled = true;
    }
    else if (faceState >= 0 && animatedObjects[faceIndex][0] != NULL && faceState <= animatedObjects[faceIndex][0]->totalFrames && faceState == faceState_)
    {
        faceState = faceState_;
        if (animatedObjects[0][animatedObjects[0][0]->currentFrame]->isFlipped != animatedObjects[faceIndex][faceState]->isFlipped)
        {
            animatedObjects[faceIndex][faceState]->FlipSprite();
        }
        animatedObjects[0][animatedObjects[0][0]->currentFrame]->AddSprite(animatedObjects[faceIndex][faceState]);
    }
}

#endif

#ifndef Tilemap_Functions

void Renderer::MakeTileMap(int** tileMapArray)
{
    ResizeBuffers();
    tileMap = tileMapArray;

    ImageBuffer* clearTile = new ImageBuffer{8,8};


    for (int x = 0; x < tileMapSize.x; ++x)
    {
        for (int y = 0; y < tileMapSize.y; ++y)
        {
            if (tileSet[tileMapArray[x][y]])
            {
                bool isWall = false;
                for (int i = 0; i < NUM_WALL_TILES; i++)
                {
                    if (tileMapArray[x][y] == wallTileIndexes[i])
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

                if (shadowCasterTileset[tileMapArray[x][y]])
                {
                    shadowCasterTileset[tileMapArray[x][y]]->position = { (float)(x * TILE_SIZE), (float)(y * TILE_SIZE) };
                    shadowCasterBuffer->AddSprite(shadowCasterTileset[tileMapArray[x][y]]);
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

void Renderer::AddShadowCasterToShadowCasterTileset(ImageBuffer* tile)
{
    shadowCasterTileset[numShadowCasterTiles] = tile;
    numShadowCasterTiles += 1;
}

#endif

#ifndef Helper_Functions

Renderer::Renderer() : objects{ NULL }
{
    outputBuffer = new ImageBuffer{ SCREEN_SIZE_X ,SCREEN_SIZE_Y };
    inputBuffer = new ImageBuffer{ SCREEN_SIZE_X ,SCREEN_SIZE_Y };
    normalBufferPostCam = new ImageBuffer{ SCREEN_SIZE_X ,SCREEN_SIZE_Y };
    lightBuffer = new ImageBuffer{ SCREEN_SIZE_X ,SCREEN_SIZE_Y };
    shadowCasterBuffer = new ImageBuffer{ SCREEN_SIZE_X ,SCREEN_SIZE_Y };
    fogBuffer = new ImageBuffer{ SCREEN_SIZE_X ,SCREEN_SIZE_Y };
    fogBufferPostCam = new ImageBuffer{ SCREEN_SIZE_X ,SCREEN_SIZE_Y };
    particleManager = new ParticleManager;
    faceIndex = -1;
    faceState = 0;
    outputBuffer->screenScale = screenScale;

    for (int i = 0; i < 15; ++i)
    {
        PreviousFrameLengths[i] = 0;
    }

    for (int i = 0; i < MAX_OBJECTS; ++i)
    {
        objects[i] = NULL;
    }

    for (int i = 0; i < MAX_ANIMATED_OBJECTS; ++i)
    {
        for (int j = 0; j < MAX_ANIMATION_FRAMES; ++j)
        {
            animatedObjects[i][j] = NULL;
        }
    }

    for (int i = 0; i < MAX_TILES; ++i)
    {
        tileSet[i] = NULL;
        normalTileSet[i] = NULL;
        shadowCasterTileset[i] = NULL;
    }

    //i hate this fix later
    ReallocateLightArrays();

    startTime = SDL_GetTicks();
    PreviousFrameBeginTime = startTime;

}

void Renderer::UpdateObjects(float dt)
{
    objectLayer->ClearImageBuffer();
    UpdateAnimations(dt);

    for (int i = 0; i < numObjects; ++i)
    {
        for (int l = 0; l < 3; ++l)
        {
            if (objects[i]->layer == l && objects[i]->totalFrames == 0 && objects[i]->isCulled == false)
            {
                objectLayer->AddSprite(objects[i]);
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
                objectLayer->AddSprite(animatedObjects[i][frame]);
                break;
            }
        }
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

void Renderer::AddObject(ImageBuffer* sprite)
{
    if ((numObjects + 1) < MAX_OBJECTS)
    {
        objects[numObjects] = sprite;
        numObjects += 1;
    }
}

int Renderer::ReturnObjectCount()
{
    int countObjects = 0;
    for (int i = 0; i < MAX_OBJECTS; ++i) {
        if (objects[i] != nullptr) {
            countObjects++;
        }
    }
    return countObjects;
}

void Renderer::ReallocateLightArrays()
{
    if (lightR != NULL)
    {
        //Free indexes
        for (int x = 0; x < SCREEN_SIZE_X; ++x) {
            delete[] lightR[x];
            delete[] lightG[x];
            delete[] lightB[x];
            delete[] blurLightR[x];
            delete[] blurLightG[x];
            delete[] blurLightB[x];
        }

        //Free main array
        delete[] lightR;
        delete[] lightG;
        delete[] lightB;
        delete[] blurLightR;
        delete[] blurLightG;
        delete[] blurLightB;
    }

    lightR = new float* [SCREEN_SIZE_X];
    lightG = new float* [SCREEN_SIZE_X];
    lightB = new float* [SCREEN_SIZE_X];

    for (int x = 0; x < SCREEN_SIZE_X; ++x)
    {
        lightR[x] = new float[SCREEN_SIZE_Y];
        lightG[x] = new float[SCREEN_SIZE_Y];
        lightB[x] = new float[SCREEN_SIZE_Y];
    }

    blurLightR = new float* [SCREEN_SIZE_X];
    blurLightG = new float* [SCREEN_SIZE_X];
    blurLightB = new float* [SCREEN_SIZE_X];

    for (int x = 0; x < SCREEN_SIZE_X; ++x)
    {
        blurLightR[x] = new float[SCREEN_SIZE_Y];
        blurLightG[x] = new float[SCREEN_SIZE_Y];
        blurLightB[x] = new float[SCREEN_SIZE_Y];
    }
}

void Renderer::LoadMenuToBuffer(const std::string filename)
{
    menuBuffer = new ImageBuffer{ filename };
}

Vector2 Renderer::GetCameraPosition(void)
{
    return CameraP;
}

void Renderer::SetCameraPosition(Vector2 NewCameraP)
{
    CameraP = NewCameraP;
}

Renderer* Renderer::GetInstance()
{
    if (instance == nullptr)
    {
        instance = new Renderer();
    }
    return instance;
}

int Renderer::CheckLineForObject(int x1, int y1, int x2, int y2)
{
    int dx = abs(x2 - x1);
    int dy = abs(y2 - y1);
    int sx = (x1 < x2) ? 1 : -1;
    int sy = (y1 < y2) ? 1 : -1;

    int error = dx - dy;
    int x = x1;
    int y = y1;

    bool inOut = false;
    int inOutCount = 0;

    while (x != x2 || y != y2)
    {
        if (inOut == false)
        {
            if (lightBuffer->SampleColor(x, y).GetAlpha() != 0)
            {
                inOutCount += 1;
                inOut = true;
            }
        }
        else
        {
            if (lightBuffer->SampleColor(x, y).GetAlpha() == 0)
            {
                inOutCount += 1;
                inOut = false;
            }
        }

        if (inOutCount > 1)
        {
            return 2;
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
    return inOutCount;
}

void Renderer::BlurLights(int blurRangeLow, int blurRangeHigh)
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

#endif

#ifndef Cleanup_Functions

Renderer::~Renderer(void)
{
    CleanRenderer();
    delete outputBuffer;
    delete inputBuffer;
    delete objectLayer;
    delete backgroundLayer;
    delete foregroundLayer;
    delete normalBuffer;
    delete normalBufferPostCam;
    delete DebugBuffer;
    delete lightBuffer;
    delete fogBuffer;
    delete fogBufferPostCam;
    delete particleManager;

    if (menuBuffer != NULL)
    {
        delete menuBuffer;
    }

    //Free indexes
    for (int x = 0; x < SCREEN_SIZE_X; ++x) {
        delete[] lightR[x];
        delete[] lightG[x];
        delete[] lightB[x];
        delete[] blurLightR[x];
        delete[] blurLightG[x];
        delete[] blurLightB[x];
    }

    //Free main array
    delete[] lightR;
    delete[] lightG;
    delete[] lightB;
    delete[] blurLightR;
    delete[] blurLightG;
    delete[] blurLightB;

    glDeleteTextures(1, &OutputBufferTexture);
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
    fogBuffer->ClearImageBuffer();
    fogBufferPostCam->ClearImageBuffer();

    if (menuBuffer != NULL)
    {
        delete menuBuffer;
        menuBuffer = NULL;
    }

    particleManager->ClearParticles();
    ReallocateLightArrays();
    ClearTilesets();
    //ClearSprites();
    //ClearLights();

    faceIndex = -1;
    numTiles = 0;
    numNormalTiles = 0;
    numObjects = 0;
    numAnimatedObjects = 0;
    numLights = 0;
    frameCount = 0;
    timer = 0;
    fogIsDirty = true;
    CameraP = Vector2{ 0,0 };
}

void Renderer::ClearLights()
{
    delete[] lightSource;
}

void Renderer::ClearSprites()
{
    for (int i = 0; i < numObjects; ++i)
    {
        if (objects[i] != NULL)
        {
            delete objects[i];
            objects[i] = NULL;
        }
    }
    numObjects = 0;
    for (int i = 0; i < numAnimatedObjects; ++i)
    {
        if (objects[i] != NULL)
        {
            for (int f = 0; f < MAX_ANIMATION_FRAMES; ++f)
            {
                delete animatedObjects[i][f];
                animatedObjects[i][f] = NULL;
            }
        }
    }
    numAnimatedObjects = 0;
    faceIndex = -1;
}

void Renderer::ClearTilesets()
{
    for (int i = 0; i < MAX_TILES; i++)
    {
        if (tileSet[i] != NULL)
        {
            delete tileSet[i];
            tileSet[i] = NULL;
        }
        if (normalTileSet[i] != NULL)
        {
            delete normalTileSet[i];
            normalTileSet[i] = NULL;
        }
        if (shadowCasterTileset[i] != NULL)
        {
            delete shadowCasterTileset[i];
            shadowCasterTileset[i] = NULL;
        }
    }
    numTiles = 0;
    numNormalTiles = 0;
    numShadowCasterTiles = 0;
}

void Renderer::ResizeBuffers()
{
    if (backgroundLayer != NULL)
    {
        delete objectLayer;
        delete backgroundLayer;
        delete foregroundLayer;
        delete normalBuffer;
        delete shadowCasterBuffer;
        delete DebugBuffer;
    }

    objectLayer = new ImageBuffer{ tileMapSize.x * (TILE_SIZE), tileMapSize.y * (TILE_SIZE) };
    backgroundLayer = new ImageBuffer{ tileMapSize.x * (TILE_SIZE), tileMapSize.y * (TILE_SIZE) };
    foregroundLayer = new ImageBuffer{ tileMapSize.x * (TILE_SIZE), tileMapSize.y * (TILE_SIZE) };
    normalBuffer = new ImageBuffer{ tileMapSize.x * (TILE_SIZE), tileMapSize.y * (TILE_SIZE) };
    shadowCasterBuffer = new ImageBuffer{ tileMapSize.x * (TILE_SIZE), tileMapSize.y * (TILE_SIZE) };
    DebugBuffer = new ImageBuffer{ tileMapSize.x * (TILE_SIZE), tileMapSize.y * (TILE_SIZE) };
}

#endif

#ifndef Debug_Functions

void Renderer::DrawLine(Vector2 P0, Vector2 P1, const Color& LineColor)
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
    if (ClippedMinX < 0)
    {
        ClippedMinX = 0;
    }
    if (ClippedMinY < 0)
    {
        ClippedMinY = 0;
    }
    if (ClippedMaxX > (DebugBuffer->size.x - 1))
    {
        ClippedMaxX = DebugBuffer->size.x - 1;
    }
    if (ClippedMaxY > (DebugBuffer->size.y - 1))
    {
        ClippedMaxY = DebugBuffer->size.y - 1;
    }

    Vector2 D = P1 - P0;
    Vector2 N = Vector2::Normalize(Vector2(-D.y, D.x));
    for (int PixelY = ClippedMinY; PixelY <= ClippedMaxY; ++PixelY)
    {
        for (int PixelX = ClippedMinX; PixelX <= ClippedMaxX; ++PixelX)
        {
            Vector2 PixelD = Vector2((float)PixelX + 0.5f, (float)PixelY + 0.5f) - P0;
            float d = fabsf(Vector2::DotProduct(N, PixelD));
            if (d <= 0.5f)
            {
                Color& DestPixel = DebugBuffer->SampleColor(PixelX, PixelY);
                DestPixel = LineColor;
            }
        }
    }
}

void Renderer::RenderWallCollidersToDebugBuffer()
{
    DebugBuffer->ClearImageBuffer();

    //make debug tile here
    ImageBuffer* debugOutlineTile = new ImageBuffer{ 8,8 };
    Color* temp;
    for (int x = 0; x < debugOutlineTile->size.x; ++x)
    {
        for (int y = 0; y < debugOutlineTile->size.y; ++y)
        {
            if (x == 0 || x == debugOutlineTile->size.x - 1 || y == debugOutlineTile->size.y - 1 || y == 0)
            {
                temp = &debugOutlineTile->SampleColor(x, y);
                *temp = Color{ 255, 0, 0, 255 };
            }
        }
    }

    const int xSize = (int)tileMapSize.x;
    const int ySize = (int)tileMapSize.y;
    //for some reason paralell makes it freak out so imma not do that even though it makes preformance a bit worse
    //#pragma omp parallel for collapse(2)
    for (int x = 0; x < xSize; ++x)
    {
        for (int y = 0; y < ySize; ++y)
        {
            bool isWalkable = true;
            for (int i = 0; i < NUM_NON_WALKABLE_TILES; i++)
            {
                if (tileMap[x][y] == nonWalkableTiles[i])
                {
                    isWalkable = false;
                    break;
                }
            }
            if (isWalkable == false)
            {
                debugOutlineTile->position = { (float)(x * TILE_SIZE), (float)(y * TILE_SIZE) };
                DebugBuffer->AddSprite(debugOutlineTile);
            }
        }
    }

    debugOutlineTile->position = animatedObjects[0][0]->position;
    DebugBuffer->AddSprite(debugOutlineTile);
}

#endif