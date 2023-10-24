//------------------------------------------------------------------------------
//
// File Name:	BehaviorPlayer.cpp
// Author(s):	Michael Howard (michael.howard)
// Purpose:		The players behavior
// 
// Copyright © 2023 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------
#include "BehaviorPlayer.h"
#include "Collider.h"
#include "Entity.h"
#include "Inputs.h"
#include "Transform.h"

BehaviorPlayer::BehaviorPlayer() : Behavior(Behavior::Player)
{
    _type = this;
}

BehaviorPlayer::BehaviorPlayer(BehaviorPlayer const& other) : Behavior(other)
{
    _type = this;
}

BehaviorPlayer::~BehaviorPlayer()
{
}

Behavior* BehaviorPlayer::Clone() const
{
	return new BehaviorPlayer(*this);
}

void BehaviorPlayer::Init()
{
    if (Parent())
    {
        // always make the behavior after the collider to avoid problems
        Collider* collider = Parent()->Has(Collider);
        if (collider)
        {
            collider->SetCollisionHandler(PlayerCollisionHandler);
        }
    }

   // input = new Inputs(Parent()->window);
}

std::string BehaviorPlayer::GetName()
{
    return Name();
}

std::string BehaviorPlayer::Name()
{
    return "BehaviorPlayer";
}

void BehaviorPlayer::Update(float dt)
{
	Controller(dt);
}

void BehaviorPlayer::SetInputHandler(Inputs* _input)
{
    input = _input;
}

void BehaviorPlayer::Read(json jsonData)
{
    UNREFERENCED_PARAMETER(jsonData);
    Init();
}


void BehaviorPlayer::Controller(float dt)
{
    Transform* transform = Parent()->Has(Transform);
    gfxVector2 translation = *transform->GetTranslation();
    input = Inputs::GetInstance();
    if (input->keyPressed(SDL_SCANCODE_W))
    {
        translation.y -= 50 * dt;
        //AudioManager.PlaySFX("footsteps.ogg");
    }
    if (input->keyPressed(SDL_SCANCODE_S))
    {
        translation.y += 50 * dt;
        //AudioManager.PlaySFX("footsteps.ogg");
    }
    if (input->keyPressed(SDL_SCANCODE_D))
    {
        if (Parent()->GetImage()->isFlipped == false)
        {
            Parent()->GetImage()->FlipSprite();
        }
        translation.x += 50 * dt;
        //AudioManager.PlaySFX("footsteps.ogg");
    }
    if (input->keyPressed(SDL_SCANCODE_A))
    {
        if (Parent()->GetImage()->isFlipped == true)
        {
            Parent()->GetImage()->FlipSprite();
        }
        translation.x -= 50 * dt;
        //AudioManager.PlaySFX("footsteps.ogg");
    }
    transform->SetTranslation(translation);
}

void BehaviorPlayer::PlayerCollisionHandler(Entity* entity1, Entity* entity2)
{
    // check which one is player and what the other one is
    // make each instance of what the player can collide 
    // with and set interactable value pressed to true if 'E' is entered
}