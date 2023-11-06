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
#include "Physics.h"
#include "Renderer.h"
#include "LevelBuilder.h"
#include "Maths/Vector.h"

const float pushForce = 1.0f;

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
    if (Parent())
	    Controller(dt);
}

void BehaviorPlayer::SetInputHandler(Inputs* _input)
{
    input = _input;
}

void BehaviorPlayer::Read(json jsonData)
{
    Init();
    playerMoveSpeed = jsonData["MoveSpeed"];
}

void BehaviorPlayer::Controller(float dt)
{
    //Renderer::GetInstance()->UpdateFace();
    Renderer::GetInstance()->faceState = 0;
    Transform* transform = Parent()->Has(Transform);
    gfxVector2 translation = *transform->GetTranslation();
    input = Inputs::GetInstance();
    if (input->keyPressed(SDL_SCANCODE_W))
    {
        Renderer::GetInstance()->faceState = 2;
        translation.y -= playerMoveSpeed * dt;
        //AudioManager.PlaySFX("footsteps.ogg");
    }
    if (input->keyPressed(SDL_SCANCODE_S))
    {
        Renderer::GetInstance()->faceState = 1;
        translation.y += playerMoveSpeed * dt;
        //AudioManager.PlaySFX("footsteps.ogg");
    }
    if (input->keyPressed(SDL_SCANCODE_D))
    {
        //Renderer::GetInstance()->faceState = 0;
        if (Parent()->GetImage()->isFlipped == false)
        {
            Parent()->GetImage()->FlipSprite();
        }
        translation.x += playerMoveSpeed * dt;
        //AudioManager.PlaySFX("footsteps.ogg");
    }
    if (input->keyPressed(SDL_SCANCODE_A))
    {
        //Renderer::GetInstance()->faceState = 0;
        if (Parent()->GetImage()->isFlipped == true)
        {
            Parent()->GetImage()->FlipSprite();
        }
        translation.x -= playerMoveSpeed * dt;
        //AudioManager.PlaySFX("footsteps.ogg");
    }
    if (!checkWalls(translation))
        transform->SetTranslation(translation);

    timer += dt;
    if (timer >= timeBetweenBlink)
    {
        Renderer::GetInstance()->faceState = 3;
        if (timer >= timeBetweenBlink + timeDuringBlink)
        {
            float range = MAX_RAND - MIN_RAND;
            timeBetweenBlink = range * ((((float)rand()) / (float)RAND_MAX)) + MIN_RAND;
            timer = 0;
        }
    }
}

bool BehaviorPlayer::checkWalls(gfxVector2 position)
{
    int* walls = LevelBuilder::GetInstance()->GetWalls();
    for (int x = 0; x < LevelBuilder::GetInstance()->GetX(); ++x)
    {
        for (int y = 0; y < LevelBuilder::GetInstance()->GetY(); ++y)
        {
            if (walls[y * LevelBuilder::GetInstance()->GetX() + x] != 0)
            {
                gfxVector2 wallworld = { (float)(x * 8), (float)(y * 8) };
                gfxVector2 playerMin, playerMax, wallMin, wallMax;
                wallMin = wallworld;
                wallMax = { wallworld.x + 8, wallworld.y + 8 };
                playerMin = { position.x + 1, position.y + 1 };
                playerMax = { position.x + 7, position.y + 7 };
                /* Check for wall collision */
                if (playerMax.x < wallMin.x || playerMin.x > wallMax.x ||
                    playerMax.y < wallMin.y || playerMin.y > wallMax.y)
                {
                    continue;
                }
                else
                {

                    // too tired will do in moring

                    //if (!(playerMax.x < wallMin.x || playerMin.x > wallMax.x))
                    //{
                    //    position.x = Parent()->Has(Physics)->GetOldTranslation().x;
                    //}
                    //else
                    //{
                    //   position.y = Parent()->Has(Physics)->GetOldTranslation().y;
                    //}

                    // Update the position to the adjusted value
                    //Transform* transform = Parent()->Has(Transform);
                    //transform->SetTranslation(position);
                    return true;
                }
            }
            
        }

    }
    return false;
    
}



void BehaviorPlayer::PlayerCollisionHandler(Entity* entity1, Entity* entity2)
{
    // check which one is player and what the other one is
    // make each instance of what the player can collide 
    // with and set interactable value pressed to true if 'E' is entered
    if (entity1->GetRealName().compare("Player") == 0 && entity2->GetRealName().compare("Object") == 0)
    {
        // Calculate the vector from object 'a' to object 'b'
        float pushDirX = entity2->GetImage()->position.x - entity1->GetImage()->position.x;
        float pushDirY = entity2->GetImage()->position.y - entity1->GetImage()->position.y;
        // Calculate the length of the vector
        float pushDirLength = sqrt(pushDirX * pushDirX + pushDirY * pushDirY);

        // Normalize the vector to obtain a unit vector
        if (pushDirLength > 0)
        {
            pushDirX /= pushDirLength;
            pushDirY /= pushDirLength;
        }

        // Apply the push force to both objects
        entity1->GetImage()->position.x -= pushDirX * pushForce;
        entity1->GetImage()->position.y -= pushDirY * pushForce;
        entity2->GetImage()->position.x += pushDirX * pushForce;
        entity2->GetImage()->position.y += pushDirY * pushForce;
    }
    if (entity1->GetRealName().compare("Player") == 0 && entity2->GetRealName().compare("Object") == 0)
    {
        // Calculate the vector from object 'a' to object 'b'
        float pushDirX = entity2->GetImage()->position.x - entity1->GetImage()->position.x;
        float pushDirY = entity2->GetImage()->position.y - entity1->GetImage()->position.y;
        // Calculate the length of the vector
        float pushDirLength = sqrt(pushDirX * pushDirX + pushDirY * pushDirY);

        // Normalize the vector to obtain a unit vector
        if (pushDirLength > 0)
        {
            pushDirX /= pushDirLength;
            pushDirY /= pushDirLength;

            // Define the minimum distance (overlap) to prevent overlap
            float overlap = 10.0f; // You can adjust this value as needed

            // Apply the push force only if there is overlap
            if (pushDirLength < overlap)
            {
                entity1->GetImage()->position.x -= pushDirX * (overlap - pushDirLength);
                entity1->GetImage()->position.y -= pushDirY * (overlap - pushDirLength);
            }
        }
    }

    
}