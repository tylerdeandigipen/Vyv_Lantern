#include "Sprite.h"
#include "Transform.h"
#include "ImageBuffer.h"

// NOTE!
// This file will be used to move things out of renderer. It works primarily as a skeleton base for now. 
// it is NOT integrated currently, and will be used for the BaseObject file later on. 

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

ImageBuffer* Animation::CreateAnimatedObject(const std::string filename, Vector2 frameSize)
{
    ImageBuffer* Result = NULL;
    ImageBuffer* spriteSheet = new ImageBuffer{ filename };

    if (spriteSheet->buffer && (frameSize.x > 0) && (frameSize.y > 0))
    {
        spriteSheet->position.y = 0;
        ImageBuffer* temp = NULL;

        for (int i = 0; i < spriteSheet->size.x / frameSize.x; i++)
        {
            temp = new ImageBuffer{ frameSize.x, frameSize.y };
            spriteSheet->position.x = -(frameSize.x * i);
            AddFrame(temp);
        }

        SetCurrentFrame(0);
        Result = frames[0];

        delete spriteSheet;
    }

    return Result;
}

//void Animation::UpdateFace(int& faceState) {}