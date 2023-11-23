#pragma once
#ifndef RENDERER_H
#define RENDERER_H

#include "ImageBuffer.h"
#include "Light.h"
#include "ParticleManager.h"
#include <SDL/SDL.h>

#define MAX_LIGHT_SOURCES 20
#define MAX_OBJECTS 60
#define MAX_ANIMATED_OBJECTS 3
#define MAX_ANIMATION_FRAMES 8
#define SCREEN_SIZE_X 240
#define SCREEN_SIZE_Y 136
#define MAX_TILES 80
#define MAX_NUM_VORNOI_POINTS 20

class Renderer
{
public:
	Renderer();
    ~Renderer(void);
    
	SDL_Window* window = NULL;

	ImageBuffer* outputBuffer = NULL;
	ImageBuffer* inputBuffer = NULL;

	ImageBuffer* foregroundLayer = NULL; //layer 2
	ImageBuffer* objectLayer = NULL; //layer 1
	ImageBuffer* backgroundLayer = NULL; //layer 0
	ImageBuffer* menuBuffer = NULL;
	ImageBuffer* shadowCasterBuffer = NULL;


	ImageBuffer* lightBuffer = NULL;
	ImageBuffer* fogBuffer = NULL;
	ImageBuffer* fogBufferPostCam = NULL;
	ImageBuffer* normalBuffer = NULL;
	ImageBuffer* normalBufferPostCam = NULL;
	Light lightSource[MAX_LIGHT_SOURCES];
	ImageBuffer* objects[MAX_OBJECTS];
	ImageBuffer* animatedObjects[MAX_ANIMATED_OBJECTS][MAX_ANIMATION_FRAMES];
	ImageBuffer* tileSet[MAX_TILES];
	ImageBuffer* normalTileSet[MAX_TILES];
	ImageBuffer* shadowCasterTileset[MAX_TILES];

	ParticleManager* particleManager = NULL;
	float screenScale = 6;
	Vector2 tileMapSize;
	bool isFullBright = false;
	bool renderNormalMap = false;
	bool renderOnlyLights = false;
	bool renderWallHitboxes = false;
	bool drawRawFog = false;
	bool doBlur = true;
	bool doScanLines = true;
	bool doFog = false;
	float scanLineOpacity = 0.935f;
	static Renderer* GetInstance();

	Vector2 GetCameraPosition(void);
	void SetCameraPosition(Vector2 NewCameraP);

	void DrawLine(Vector2 P0, Vector2 P1, const Color &LineColor);

	void RenderLightingPass();
	bool CalculateIfPixelIsLit(int x, int y, int i);
	float FindPixelLuminosity(float x, float y, Light *LightSource);

	ImageBuffer* GetObjectByName(std::string name_);
    
	void BlurLights();
	void ResizeBuffers();
	void RenderParticles();
	void MakeTileMap(int** tileMapArray);
	void AddTileToTileset(ImageBuffer* tile);
	void AddNormalToNormalTileset(ImageBuffer* tile);
	void AddShadowCasterToShadowCasterTileset(ImageBuffer* tile);
	void AddObject(ImageBuffer* sprite);
	ImageBuffer* CreateAnimatedObject(const std::string filename, Vector2 frameSize);
	void AddLight(Light light);
	void UpdateAnimations(float dt);
	void UpdateObjects();
	void Update(float dt);
	int returnObjCnt();
	void ClearTilesets();
	void ClearSprites();
	void ClearLights();
	void RenderWallCollidersToDebugBuffer();
	int CheckLineForObject(int x1, int y1, int x2, int y2);
	// 0 = forward, 1 = down, 2 = up, 3 = blink
	void UpdateFace(int& faceState_);
	void CleanRenderer();
	void DitherBuffer(ImageBuffer* input, ImageBuffer* output, bool useLightIntensity = false);
	void RenderToOutbuffer();
	void RenderMenu();
	void ReallocateLightArrays();
	void MakeMenu(const std::string filename);
	void CalculateShadows();
	void MakeVornoiNoiseBuffer(ImageBuffer* out, float brightnessModifier, float brightnessMultiplier, float minBrightness);
	void GenerateVornoiPoints();
	Color BlendColors(Color top, Color bottom, float p);
	void BlurBuffer(ImageBuffer* buffer);
	void FloorBrightness(ImageBuffer* buffer, float floor);
	int faceState = 0;
	float normalStrength = 0.5f;
	//i hate this make it better later tho
	float** lightR = NULL;
	float** lightG = NULL;
	float** lightB = NULL;

	float** blurLightR = NULL;
	float** blurLightG = NULL;
	float** blurLightB = NULL;

private:
	static Renderer* instance;

	Vector2 vornoiPoints[MAX_NUM_VORNOI_POINTS] = {Vector2{0,0}};
	bool fogIsDirty = true;
	Vector2 fogOffSet{0,0};
	Vector2 fogPos{0,0};
	Vector2 fogMoveDir{0.08, 0.01};
	float fogOpacity = 5;

	ImageBuffer *DebugBuffer;
	int faceIndex = -1;
	Vector2 CameraP;
	int numTiles = 0;
	int numNormalTiles = 0;
	int numShadowCasterTiles = 0;
	int numObjects = 0;
	int numAnimatedObjects = 0;
	int numLights = 0;
	int frameCount = 0;
	float timer = 0;
	float timeBetweenFrames = 0.1f;
	int** tileMap = NULL;

    Uint32 startTime;
    
	uint32_t OutputBufferTexture = NULL;
    uint32_t PreviousFrameBeginTime = NULL;
    float PreviousFrameLengths[15];
};

#endif 
