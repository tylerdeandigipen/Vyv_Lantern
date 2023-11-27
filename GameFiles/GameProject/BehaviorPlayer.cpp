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
#include "Maths/Math.h"
#include "Maths/Vector.h"
#include "AudioEngine.h"

const float pushForce = 1.0f;

BehaviorPlayer::BehaviorPlayer() : Behavior(Behavior::Player), playerMoveSpeed(0)
{
    _type = this;
}

BehaviorPlayer::BehaviorPlayer(BehaviorPlayer const& other) : Behavior(other), playerMoveSpeed(other.playerMoveSpeed)
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
    Inputs* input = Inputs::GetInstance();
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

	moveEntityTowards(translation);

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

inline void
testWallAxis(float wallMinX, float wallMaxX, float wallMinY, float wallMaxY,
			 float playerX, float playerY, float playerDeltaX, float playerDeltaY,
			 Vector2 wallMinEdgeNormal, Vector2 wallMaxEdgeNormal, 
			 float *tMin, Vector2 *collisionNormal)
{
	float denom = playerDeltaX;
	if(denom != 0.0f)
	{
		float tX0 = (wallMinX - playerX) / denom;
		float tX1 = (wallMaxX - playerX) / denom;
		float tX = min(tX0, tX1);

		Vector2 wallNormal = wallMinEdgeNormal;
		if(tX1 < tX0)
		{
			wallNormal = wallMaxEdgeNormal;
		}

		if((tX >= 0.0f) && (*tMin > tX))
		{
			float testY = playerY + (tX * playerDeltaY);
			if((testY < wallMaxY) && (testY > wallMinY))
			{
				*tMin = tX;
				*collisionNormal = wallNormal;
			}
		}
	}
}


void BehaviorPlayer::moveEntityTowards(Vector2 targetPosition)
{
	Transform* transform = Parent()->Has(Transform);
	Vector2 oldPosition = *transform->GetTranslation();
	Vector2 newPosition = oldPosition;

	Vector2 playerDisplacement = targetPosition - oldPosition;
    float playerRadius = Parent()->GetImage()->BufferSizeX / 2.0f;

#define TILE_SIZE_IN_PIXELS 8
	int minTileX = (int)((oldPosition.x - TILE_SIZE_IN_PIXELS) / TILE_SIZE_IN_PIXELS);
	int minTileY = (int)((oldPosition.y - TILE_SIZE_IN_PIXELS) / TILE_SIZE_IN_PIXELS);
	int maxTileX = (int)ceilf((targetPosition.x + TILE_SIZE_IN_PIXELS) / TILE_SIZE_IN_PIXELS);
    int maxTileY = (int)ceilf((targetPosition.y + TILE_SIZE_IN_PIXELS) / TILE_SIZE_IN_PIXELS);

    int mapWidth = LevelBuilder::GetInstance()->GetX();
    int mapHeight = LevelBuilder::GetInstance()->GetY();
    
    minTileX = max(0, minTileX);
    minTileY = max(0, minTileY);
    maxTileX = min(mapWidth - 1, maxTileX);
    maxTileY = min(mapHeight - 1, maxTileY);

    Vector2 collisionNormal = Vector2(0, 0);
    
    float tRemaining = 1.0f;
	int iterationCount = 4;
    while((iterationCount--) && (tRemaining > 0.0f))
    {
        float tMin = 1.0f;
        int *wallTiles = LevelBuilder::GetInstance()->GetWalls();
        
        for(int Y = minTileY; Y <= maxTileY; ++Y)
        {
            for(int X = minTileX; X <= maxTileX; ++X)
            {
                int tileValue = wallTiles[(Y * mapWidth) + X];
                if(tileValue)
                {
                    Vector2 wallMinP = {TILE_SIZE_IN_PIXELS*(float)X, TILE_SIZE_IN_PIXELS*(float)Y};
                    Vector2 wallMaxP = wallMinP + Vector2(TILE_SIZE_IN_PIXELS, TILE_SIZE_IN_PIXELS);
                    wallMinP.x -= playerRadius;
                    wallMinP.y -= playerRadius;
					wallMinP.x -= playerRadius;
					wallMinP.y -= playerRadius;

					testWallAxis(wallMinP.x, wallMaxP.x, wallMinP.y, wallMaxP.y,
								 newPosition.x, newPosition.y, 
								 playerDisplacement.x, playerDisplacement.y,
								 Vector2(-1, 0), Vector2(1, 0),
								 &tMin, &collisionNormal); 
					testWallAxis(wallMinP.y, wallMaxP.y, wallMinP.x, wallMaxP.x,
								 newPosition.y, newPosition.x, 
								 playerDisplacement.y, playerDisplacement.x,
								 Vector2(0, -1), Vector2(0, 1),
								 &tMin, &collisionNormal);
                }
            }
        }
        
        newPosition += (tMin*playerDisplacement);
        playerDisplacement -= Vector2::DotProduct(collisionNormal, playerDisplacement)*collisionNormal;

        tRemaining -= tMin*tRemaining;
    }
    
	if(transform)
	{
		transform->SetTranslation(newPosition);
	}
#undef TILE_SIZE_IN_PIXELS
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
