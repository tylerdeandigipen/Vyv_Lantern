#pragma once
#ifndef RENDERER_H
#define RENDERER_H

#include "ImageBuffer.h"
#include "Light.h"
#include <SDL/SDL.h>

#define MAX_LIGHT_SOURCES 20
#define MAX_OBJECTS 20
#define SCREEN_SIZE_X 240
#define SCREEN_SIZE_Y 135

class Renderer
{
public:
	Renderer();
    ~Renderer(void);
    
	SDL_Window* window;

	ImageBuffer* outputBuffer;
	ImageBuffer* tileMapLayer; //layer 2
	ImageBuffer* objectLayer; //layer 1
	ImageBuffer* backgroundLayer; //layer 0
	ImageBuffer* bakedLightsBuffer;
	ImageBuffer* inputBuffer;

	Light lightSource[MAX_LIGHT_SOURCES];
	ImageBuffer* objects[MAX_OBJECTS];

	float screenScale = 6;
	float bakedVolumetricIntensity = .25f;

	Vector2 GetCameraPosition(void);
	void SetCameraPosition(Vector2 NewCameraP);

	void RenderLightingPass();
	float FindPixelLuminosity(float x, float y, int i, Light lightSource_[MAX_LIGHT_SOURCES]);

	ImageBuffer* GetObjectByName(std::string name_);
    
	void MakeTileMap(int tileMapArray[16][9]);
	void AddObject(ImageBuffer* sprite);
	void AddLight(Light light);
	void UpdateObjects();
	void Update();
	int returnObjCnt();

private:

	Vector2 CameraP;

	int numObjects;
	int numLights;
	int numStaticLights;

	int frameCount = 0;

    Uint32 startTime;
    
	uint32_t OutputBufferTexture;
    uint32_t PreviousFrameBeginTime;
    float PreviousFrameLengths[15];
    
	bool LightsBaked = false;
};

#endif 
