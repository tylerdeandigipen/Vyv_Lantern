#include "BaseObject.h"

BaseObject::BaseObject() : transform(), sprite()
{
}

void BaseObject::Update(float dt)
{
   
}

void BaseObject::Render()
{
   
}

Transform& BaseObject::GetTrans()
{
    return transform;
}

Sprite& BaseObject::GetSprite()
{
    return sprite;
}
