#pragma once
#ifndef RENDERER_H
#define RENDERER_H
#include "ImageBuffer.h"
#include "Light.h"
#include <SDL/SDL.h>

#define MAX_LIGHT_SOURCES 20
#define MAX_OBJECTS 20

class Renderer
{
public:
	Renderer();
	SDL_Window* window;
	SDL_Renderer* renderer;
	ImageBuffer* outputBuffer;
	ImageBuffer* objectLayer;
	ImageBuffer* backgroundLayer;
	ImageBuffer* bakedLightsBuffer;
	ImageBuffer* inputBuffer;
	Light lightSource[MAX_LIGHT_SOURCES];
	Light staticLightSource[MAX_LIGHT_SOURCES];
	ImageBuffer* objects[MAX_OBJECTS];
	float screenScale = 6;

	void RenderLightingPass();
	ImageBuffer* GetObjectByName(std::string name_);
	void AddObject(ImageBuffer* sprite);
	void AddLight(Light light);
	void UpdateObjects();
	void BakeLights();
	void Update();
private:
	int numObjects;
	int numLights;
	int numStaticLights;
};

#endif 