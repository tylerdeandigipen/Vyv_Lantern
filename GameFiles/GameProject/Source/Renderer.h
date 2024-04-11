//------------------------------------------------------------------------------
//
// File Name:	Renderer.h
// Author(s):	Tyler Dean, Thomas Stephenson, Louis Wang
// Purpose:		Handles all rendering tasks.
//
// Copyright ?2023 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------
#pragma once
#ifndef RENDERER_H
#define RENDERER_H

#include "ImageBuffer.h"
#include "TBDLasers.h"
#include "Light.h"
#include "ParticleManager.h"
#include <SDL/SDL.h>

#define MAX_LIGHT_SOURCES 50
#define MAX_OBJECTS 60
#define MAX_ANIMATED_OBJECTS 3
#define MAX_ANIMATION_FRAMES 8
#define SCREEN_SIZE_X 240
#define SCREEN_SIZE_Y 136
#define MAX_TILES 94
#define TILE_SIZE 8
#define NUM_WALL_TILES 70
#define NUM_NON_WALKABLE_TILES 70
#define MAX_LASER_POINTS 100
#define MAX_DECALS 100
#define MAX_MENU_PAGES 10

class Renderer
{
public:

	int numMenuPages = 0;

	//Visual Effects Settings
	float screenScale = 6; //doesnt work for some reason, havent looked into yet bc not too important
	float saturationPercent = 1;
	float normalStrength = 0.5f;
	bool doBlur = true;
	bool doScanLines = true;
	float scanLineOpacity = 0.935f;
	bool doFog = false;
	Vector2 fogMoveDir{ 0.08f, 0.01f };
	float fogOpacity = 5;
	float minBrightness = 34;

	//Debug Draw Toggles
	bool isFullbright = false;
	bool renderNormalMap = false;
	bool renderOnlyLights = false;
	bool renderWallHitboxes = false;
	bool drawRawFog = false;

	//Update \/ (its the update function(it updates things))
	void Update(float dt);

	//Rendering Functions
	void RenderToOutbuffer();
	void RenderLightingPass();
	float FindPixelLuminosity(float x, float y, Light* LightSource);
	void CalculateShadows();
	bool CalculateIfPixelIsLit(int x, int y, int i);
	void RenderMenu();
	void RenderParticles();
	void RenderFog();
	void RenderLasers();
	void DrawLaserLines(int thickness);

	//Animation Functions
	void UpdateAnimations(float dt);
	ImageBuffer* CreateAnimatedObject(const std::string filename, Vector2 frameSize);
	void TintScreenBlack(float progress);

	// 0 = forward, 1 = down, 2 = up, 3 = blink
	void UpdateFace(int faceState_);

	//Tilemap Functions
	void MakeTileMap(int** tileMapArray);
	void AddDecal(ImageBuffer* sprite, Vector2 pos);
	void AddDecalsToTilemap();
	void FreeDecalFromRenderer(ImageBuffer* spriteToFree);
	void TileMapSetTile(Vector2 pos, int tile);
	void TileMapEraseTile(Vector2 pos);
	void AddTileToTileset(ImageBuffer* tile);
	void AddNormalToNormalTileset(ImageBuffer* tile);
	void AddShadowCasterToShadowCasterTileset(ImageBuffer* tile);

	//Helper Functions
	void UpdateObjects(float dt);
	ImageBuffer* GetObjectByName(std::string name_);
	int AddMenuPage(const std::string filename);
	void LoadMenuPage(int index);
	void ClearMenuPages();
	void AddLight(Light light);
	void AddObject(ImageBuffer* sprite);
	int ReturnObjectCount();
	void ReallocateLightArrays();
	void LoadMenuToBuffer(const std::string filename);
	Vector2 GetCameraPosition(void);
	void SetCameraPosition(Vector2 NewCameraP);
	static Renderer* GetInstance();
	int CheckLineForObject(int x1, int y1, int x2, int y2);
	gfxVector2 LaserCheckLineForObject(Vector2 pos1, Vector2 pos2);
	void BlurLights(int blurRangeLow, int blurRangeHigh);
	void BlurLasers(int blurRangeLow, int blurRangeHigh);
	void ExpandTileMapInDirection(Vector2 direction, int distance);

	auto GetTileCount() -> unsigned int;

	//Cleanup Functions
	~Renderer(void);
	void CleanRenderer();
	void ClearLights();
	void ClearSprites();
	void ClearTilesets();
	void ResizeBuffers();

	//Debug Functions
	void DrawLine(Vector2 P0, Vector2 P1, const Color& LineColor, ImageBuffer* buffer = NULL, float skipPercent = 0);
	void RenderWallCollidersToDebugBuffer();

	//Output Buffer
	ImageBuffer* outputBuffer = NULL;

	//Core Layers
	ImageBuffer* menuBuffer = NULL; // layer 3 (menu / ui)
	ImageBuffer* foregroundLayer = NULL; //layer 2 (infront of player)
	ImageBuffer* objectLayer = NULL; //layer 1 (player / interactables)
	ImageBuffer* backgroundLayer = NULL; //layer 0 (behind player)

	//Visual Effect Buffers
	ImageBuffer* lightBuffer = NULL;
	ImageBuffer* laserBuffer = NULL;
	ImageBuffer* shadowCasterBuffer = NULL;
	ImageBuffer* normalBuffer = NULL;

	//ImageBuffer* fogBuffer = NULL;

	//Object References
	Light lightSource[MAX_LIGHT_SOURCES];
	ImageBuffer* objects[MAX_OBJECTS];
	ImageBuffer* animatedObjects[MAX_ANIMATED_OBJECTS][MAX_ANIMATION_FRAMES];
	ImageBuffer* menuPages[MAX_MENU_PAGES] = { NULL };

	//Needed to be public (maybe make private later by adding helper funcs)
	Vector2 tileMapSize;
	ParticleManager* particleManager = NULL;
	SDL_Window* window = NULL;

	//i hate this make it better later tho
	float** lightR = NULL;
	float** lightG = NULL;
	float** lightB = NULL;

	float** blurLightR = NULL;
	float** blurLightG = NULL;
	float** blurLightB = NULL;

	int** tileMap = NULL;

	int nonWalkableTiles[NUM_NON_WALKABLE_TILES] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 26, 27, 28, 29, 30, 31, 41, 42, 43, 45, 46, 47, 48, 49, 50, 51,52,53,54,58,62,63,69,76,77,82,88,90,91 };
	int numNonWalkTiles = NUM_NON_WALKABLE_TILES;

	int currentMenu = -1;

	Vector2 laserPoints1[MAX_LASER_POINTS];
	Vector2 laserPoints2[MAX_LASER_POINTS];
	Color laserColor[MAX_LASER_POINTS];
	int numLasers = 0;
	TBDLasers laserHandler; // handler for mirrors and lasers
	const int GetNumLights() const { return numLights; };
private:
	Renderer();

	//Various Counters
	int numTiles = 0;
	int numNormalTiles = 0;
	int numShadowCasterTiles = 0;
	int numObjects = 0;
	int numAnimatedObjects = 0;
	int numLights = 0;

	//Fog Variables
	bool fogIsDirty = true;
	Vector2 fogOffSet{ 0,0 };
	Vector2 fogPos{ 0,0 };

	//Face Variables
	int faceIndex = -99;

	//Tilemap Variables
	const int wallTileIndexes[NUM_WALL_TILES] = { 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 26, 27, 28, 29, 30, 31 };
	ImageBuffer* tileSet[MAX_TILES];
	ImageBuffer* normalTileSet[MAX_TILES];
	ImageBuffer* shadowCasterTileset[MAX_TILES];
	ImageBuffer* decals[MAX_DECALS];
	Vector2 decalPositions[MAX_DECALS];
	int numDecals = 0;
	bool decalsAreDirty = false;

	//Framerate Variables
	int frameCount = 0;
	float timer = 0;
	float timeBetweenFrames = 0.1f;
	Uint32 startTime;
	uint32_t PreviousFrameBeginTime = NULL;
	float PreviousFrameLengths[15];

	//Misc.
	float fadePercent = 1;
	int faceState = 0;
	unsigned int maxThreadsAllowed = 8;
	Vector2 nextCamPos;
	Vector2 CameraP;
	static std::unique_ptr<Renderer> instance;
	ImageBuffer* DebugBuffer = NULL;
	uint32_t OutputBufferTexture = NULL;
};

#endif
