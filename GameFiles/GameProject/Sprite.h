#pragma once
#include "Renderer.h"

class Animation
{
	Animation();
	virtual ~Animation() = default;

	void Update(float dt);
	void AddFrame(ImageBuffer* frame);
	void SetCurrentFrame(int index);

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