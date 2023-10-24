#pragma once
#ifndef RENDERER_H
#define RENDERER_H

#include "ImageBuffer.h"
#include "Light.h"
#include <SDL/SDL.h>

#define MAX_LIGHT_SOURCES 20
#define MAX_OBJECTS 60
#define MAX_ANIMATED_OBJECTS 3
#define MAX_ANIMATION_FRAMES 8
#define SCREEN_SIZE_X 240
#define SCREEN_SIZE_Y 136

class Renderer
{
public:
	Renderer();
    ~Renderer(void);
    
	SDL_Window* window;

	ImageBuffer* outputBuffer;
	ImageBuffer* inputBuffer;

	ImageBuffer* foregroundLayer; //layer 2
	ImageBuffer* objectLayer; //layer 1
	ImageBuffer* backgroundLayer; //layer 0

	Light lightSource[MAX_LIGHT_SOURCES];
	ImageBuffer* objects[MAX_OBJECTS];
	ImageBuffer* animatedObjects[MAX_ANIMATED_OBJECTS][MAX_ANIMATION_FRAMES];
	float screenScale = 6;
	Vector2 tileMapSize;
	bool isFullBright = false;

	Vector2 GetCameraPosition(void);
	void SetCameraPosition(Vector2 NewCameraP);

	void DrawLine(Vector2 P0, Vector2 P1, const Color &LineColor);

	void RenderLightingPass();
	float FindPixelLuminosity(float x, float y, Light *LightSource);

	ImageBuffer* GetObjectByName(std::string name_);
    
	void ResizeBuffers();
	void MakeTileMap(int** tileMapArray);
	void AddTileToTileset(ImageBuffer* tile);
	void AddObject(ImageBuffer* sprite);
	void AddAnimatedObject(const std::string filename, Vector2 frameSize);
	ImageBuffer* CreateAnimatedObject(const std::string filename, Vector2 frameSize);
	void AddLight(Light light);
	void UpdateAnimations(float dt);
	void UpdateObjects();
	void Update();
	int returnObjCnt();
	void brensenhamalgo(int x1, int y1, int x2, int y2);
private:

	ImageBuffer *DebugBuffer;

	Vector2 CameraP;
	ImageBuffer* tileSet[32];
	int numTiles = 0;
	int numObjects = 0;
	int numAnimatedObjects = 0;
	int numLights = 0;
	int frameCount = 0;
	float timer = 0;
	float timeBetweenFrames = 0.1f;

    Uint32 startTime;
    
	uint32_t OutputBufferTexture;
    uint32_t PreviousFrameBeginTime;
    float PreviousFrameLengths[15];
};

#endif 
