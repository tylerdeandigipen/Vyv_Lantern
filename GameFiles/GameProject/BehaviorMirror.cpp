#include "BehaviorMirror.h"
#include "Collider.h"
#include "Entity.h"
#include "Inputs.h"
#include "Transform.h"
#include "Physics.h"
#include "Renderer.h"
#include "LevelBuilder.h"
#include "Maths/Vector.h"
#include "AudioEngine.h"
#include "Collision.h"
#include <cassert>
#include <cmath>
#include <iostream>
#include "EntityContainer.h"
#include "BehaviorSwitch.h"

std::vector<gfxVector2> BehaviorMirror::pos;
int BehaviorMirror::count = 0;
int BehaviorMirror::maxCount = 4;
gfxVector2 BehaviorMirror::currentPos = gfxVector2(0, 0);
gfxVector2 BehaviorMirror::targetPos = gfxVector2(0, 0);

BehaviorMirror::BehaviorMirror() : Behavior(Behavior::Mirror)
{
    _type = this;
}

BehaviorMirror::BehaviorMirror(BehaviorMirror const& other) : Behavior(other)
{
    _type = this;
}

BehaviorMirror::~BehaviorMirror()
{
}

std::string BehaviorMirror::GetName()
{
    return Name();
}

std::string BehaviorMirror::Name()
{
    return "BehaviorMirror";
}

void BehaviorMirror::SetInputHandler(Inputs* _input)
{
}

void BehaviorMirror::Init()
{


    if (Parent())
    {
        // Set collision handler for mirrors with lasers maybe??
        Collider* collider = Parent()->Has(Collider);
    }
}

Behavior* BehaviorMirror::Clone() const
{
    return new BehaviorMirror(*this);
}

void BehaviorMirror::Update(float dt)
{
    if (Parent())
        Controller(dt);
}

void BehaviorMirror::Read(json jsonData)
{
    Init();
    for (auto& positions : jsonData["pos"])
    {
        // Extract "x" and "y" values, convert them to integers, and store in the vector
        float x = std::stoi(positions["x"].get<std::string>());
        float y = std::stoi(positions["y"].get<std::string>());

        pos.push_back({ x,y });
    }
}

void BehaviorMirror::SwitchOn(bool collided)
{
    if (collided == true)
    {
        const EntityContainer entityContainer = *LevelBuilder::GetInstance()->GetContainer();
        int numEntities = LevelBuilder::GetInstance()->CountEntities();

        for (int i = 0; i < numEntities; i++) {
            Entity* entity = entityContainer[i];
            if (entity->GetRealName() == "Mirror") {
                if (count < maxCount) {
                    targetPos = pos[count]; // Update target position
                    count++;
                }
                else {
                    count = 0;
                    targetPos = pos[count]; // Update target position
                }
            }
        }
    }
}

void BehaviorMirror::MirrorCollisionHandler(Entity* thisone, Entity* other)
{
}

void BehaviorMirror::Controller(float dt)
{

    float lerpFactor = 0.5f; // Adjust this value for speed

    currentPos = currentPos + lerpFactor * (targetPos - currentPos);

    if (Parent() && Parent()->Has(Transform)) {
        Parent()->GetImage()->position = currentPos;
    }
}