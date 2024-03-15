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
#include "Component.h"
#include "Renderer.h"
#include "ImageBuffer.h"

class Animation : public Component
{
	Animation();
	Animation(Animation const& rhs);
	~Animation();

	// used to invoke the copy constructor
	Component* Clone(void) const;

	//component-specific render code.

	void Render() const {};
	void Read(json jsonData);

	std::string GetName() { return "Component"; };
	void Update(float dt);
	void AddFrame(ImageBuffer* frame);
	void SetCurrentFrame(int index);

	ImageBuffer* CreateAnimatedObject(const std::string filename, Vector2 frameSize);

private:
	std::vector<ImageBuffer*> frames;
	int currentFrame;
	float timer;
	float timeBetweenFrames;
};