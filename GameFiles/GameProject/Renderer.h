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
	SDL_Window* window;
	SDL_Renderer* renderer;
	ImageBuffer* outputBuffer;
	ImageBuffer* inputBuffer;
	Light lightSource[MAX_LIGHT_SOURCES];
	ImageBuffer* objects[MAX_OBJECTS];

	void RenderLightingPass();
	ImageBuffer* GetObjectByName(std::string name_);
	void AddObject(ImageBuffer* sprite);
	void AddLight(Light light);
	void UpdateObjects();
	void Update();
	void Init();
private:
	int numObjects;
	int numLights;
};

#endif 