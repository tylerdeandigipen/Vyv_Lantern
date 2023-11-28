#include "Sprite.h"
#include "Transform.h"

Sprite::Sprite()
{
}

Animation::Animation() : frames(), currentFrame(0), timer(0.0f), timeBetweenFrames(0.1f)
{

}

void Sprite::Render(/*const Transform& transform*/)
{
    
}

void Animation::Update(float dt)
{
    timer += dt;

    if (timer >= timeBetweenFrames)
    {
        currentFrame = (currentFrame + 1) % frames.size();
        timer = 0.0f;
    }
}

void Animation::AddFrame(ImageBuffer* frame)
{
    frames.push_back(frame);
}

void Animation::SetCurrentFrame(int index)
{
    if (index >= 0 && index < frames.size())
    {
        currentFrame = index;
    }
}