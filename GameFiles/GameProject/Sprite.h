//------------------------------------------------------------------------------
//
// File Name:	Sprite.h
// Author(s):	TayLee Young
// Purpose:		Sprite class declaration to handle sprites and animations. 
//
// Copyright © 2023 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------
#pragma once
#include "Renderer.h"
#include "ImageBuffer.h"

class Animation
{
	Animation();
	virtual ~Animation() = default;

	void Update(float dt);
	void AddFrame(ImageBuffer* frame);
	void SetCurrentFrame(int index);

	ImageBuffer* CreateAnimatedObject(const std::string filename, Vector2 frameSize);
	void UpdateFace(int& faceState);

private:
	std::vector<ImageBuffer*> frames;
	int currentFrame;
	float timer;
	float timeBetweenFrames;
};

class Sprite
{
public:
	Sprite();
	virtual ~Sprite() = default;

	void Render(/*const Transform& transform*/);

private:
	Animation* animation;
};