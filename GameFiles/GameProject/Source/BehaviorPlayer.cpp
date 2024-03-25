//------------------------------------------------------------------------------
//
// File Name:	BehaviorPlayer.cpp
// Author(s):	Michael Howard (michael.howard), Thomas Stephenson, Louis Wang
// Purpose:		The players behavior
// 
// Copyright ?2023 DigiPen (USA) Corporation.
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
float soundCooldown = 0.0f;

BehaviorPlayer::BehaviorPlayer() : Behavior(Behavior::bPlayer), playerMoveSpeed(0)
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
    {
        Controller(dt);
        
        SetGoUp(true);
        SetGoDown(true);
        SetGoRight(true);
        SetGoLeft(true);
    }
}

void BehaviorPlayer::Read(json jsonData)
{
    Init();
    playerMoveSpeed = jsonData["MoveSpeed"];
}

void BehaviorPlayer::Controller(float dt)
{

    if (Engine::GetInstance()->Paused() == false)
    {
    faceState = 0;
    Transform* transform = Parent()->Has(Transform);
    gfxVector2 translation = *transform->GetTranslation();
    if (realPos == Vector2(-9999, -9999))
    {
        realPos = translation;
    }
    translation = realPos;
    Inputs* input = Inputs::GetInstance();
    if (input->keyPressed(SDL_SCANCODE_W))
    {
        faceState = 2;
        if (isUp)
            translation.y -= playerMoveSpeed * dt;
        //AudioManager.PlaySFX("footsteps.ogg");
    }
    if (input->keyPressed(SDL_SCANCODE_S))
    {
        faceState = 1;
        if (isDown)
            translation.y += playerMoveSpeed * dt;
        //AudioManager.PlaySFX("footsteps.ogg");
    }
    if (input->keyPressed(SDL_SCANCODE_D))
    {
        if (Parent()->GetImage()->isFlipped == false)
        {
            Parent()->GetImage()->FlipSprite();
        }
        if (isRight)
            translation.x += playerMoveSpeed * dt;
        //AudioManager.PlaySFX("footsteps.ogg");
    }
    if (input->keyPressed(SDL_SCANCODE_A))
    {
        if (Parent()->GetImage()->isFlipped == true)
        {
            Parent()->GetImage()->FlipSprite();
        }
        if (isLeft)
            translation.x -= playerMoveSpeed * dt;
        //AudioManager.PlaySFX("footsteps.ogg");
    }

    if (soundCooldown <= 0.0f)
    {
        AudioManager.PlaySFX("footsteps.ogg");
        soundCooldown = 1.0f; // Set the cooldown time
    }

	moveEntityTowards(translation);

	timer += dt;
    if (timer >= timeBetweenBlink)
    {
        faceState = 3;
        if (timer >= timeBetweenBlink + timeDuringBlink)
        {
            float range = MAX_RAND - MIN_RAND;
            timeBetweenBlink = range * ((((float)rand()) / (float)RAND_MAX)) + MIN_RAND;
            timer = 0;
        }
    }
    Renderer::GetInstance()->UpdateFace(faceState);

    soundCooldown -= dt;
    if (soundCooldown < 0.0f) {
        soundCooldown = 0.0f;
    }


        int x, y;
        Uint32 buttons = SDL_GetMouseState(&x, &y);
        UNREFERENCED_PARAMETER(buttons);
        //This is the Scenes Player light tracking code or at least part of it
        // assuming Cursour Player and Light Player are the naming schemes.
        //no chnges needed
        Vector2 CursourP = { (float)x, (float)y };
        CursourP *= 1.0f / Renderer::GetInstance()->screenScale;
        CursourP += Renderer::GetInstance()->GetCameraPosition();


        //Get properlightsources incase of global changes
        Vector2 LightP = Renderer::GetInstance()->lightSource[0].position;
        Vector2 D = LightP - CursourP;
        float Angle = atan2f(D.x, D.y) * (180.0f / 3.14f) + 180.0f;
        Renderer::GetInstance()->lightSource[0].angle = Angle;
        ImageBuffer* playerEntity = Renderer::GetInstance()->animatedObjects[0][0];
        if (centerCameraOnPlayer && playerEntity->isCulled != true)
        {
            Vector2 ScreenHalfSize = 0.5f * Vector2(SCREEN_SIZE_X, SCREEN_SIZE_Y);
            Vector2 BitmapHalfDim = 0.5f * playerEntity->size;
            Vector2 newCamPos = (playerEntity->position - ScreenHalfSize + BitmapHalfDim) + ((CursourP - Renderer::GetInstance()->GetCameraPosition() - ScreenHalfSize + BitmapHalfDim) * followCursorScalar);
            Renderer::GetInstance()->SetCameraPosition(newCamPos);
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
    if (realPos == Vector2(-9999, -9999))
    {
        realPos = oldPosition;
    }
    oldPosition = realPos;
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
        realPos = newPosition;
        newPosition.x = std::truncf(newPosition.x);
        newPosition.y = std::truncf(newPosition.y);

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

    if (entity1->GetRealName().compare("Player") == 0 && entity2->GetRealName().compare("Door") == 0)
    {
        auto door = reinterpret_cast<BehaviorDoor*>(entity2->Has(Behavior));
        //could we perhaps use a better name so that we don't hide a class member?
        auto player = reinterpret_cast<BehaviorPlayer*>(entity1->Has(Behavior));
        if (door->GetDoorClosed())
        {
            if (entity1->GetImage()->position.x + 2 < entity2->GetImage()->position.x)
                player->SetGoRight(false);
            if (entity1->GetImage()->position.x > entity2->GetImage()->position.x + 8)
                player->SetGoLeft(false);
            if (entity1->GetImage()->position.y + 2 < entity2->GetImage()->position.y)
                player->SetGoDown(false);
            if (entity1->GetImage()->position.y > entity2->GetImage()->position.y + 8)
                player->SetGoUp(false);
        }
        //float pushDirX = (entity2->GetImage()->position.x + 5.f) - entity1->GetImage()->position.x;
        //float pushDirY = (entity2->GetImage()->position.y + 5.f) - entity1->GetImage()->position.y;

        //float pushDirLength = sqrt(pushDirX * pushDirX + pushDirY * pushDirY);

        //float overlap = 25.0f; // You can adjust this value as needed

        //if (door->GetDoorClosed())
        //{
        //    // Apply the push force only if there is overlap
        //    if (pushDirLength > 0)
        //    {
        //        pushDirX /= pushDirLength;
        //        pushDirY /= pushDirLength;

        //        // Apply the push force only if there is overlap
        //        if (pushDirLength < overlap)
        //        {
        //            entity1->GetImage()->position.x -= (pushDirX * (overlap - pushDirLength)) / 3.f;
        //            entity1->GetImage()->position.y -= (pushDirY * (overlap - pushDirLength)) / 3.f;
        //        }
        //    }
        //}
    }
    if (entity2->GetRealName().compare("Player") == 0 && entity1->GetRealName().compare("Door") == 0)
    {
        auto door = reinterpret_cast<BehaviorDoor*>(entity1->Has(Behavior));
        auto player = reinterpret_cast<BehaviorPlayer*>(entity2->Has(Behavior));

        if (door->GetDoorClosed())
        {
            if (entity2->GetImage()->position.x + 2 < entity1->GetImage()->position.x)
                player->SetGoRight(false);
            if (entity2->GetImage()->position.x > entity1->GetImage()->position.x + 8)
                player->SetGoLeft(false);
            if (entity2->GetImage()->position.y + 2 < entity1->GetImage()->position.y)
                player->SetGoDown(false);
            if (entity2->GetImage()->position.y > entity1->GetImage()->position.y + 8)
                player->SetGoUp(false);
        }
        //auto door = reinterpret_cast<BehaviorDoor*>(entity1->Has(Behavior));

        //float pushDirX = (entity1->GetImage()->position.x) / 2.f - entity2->GetImage()->position.x;
        //float pushDirY = (entity1->GetImage()->position.y) / 2.f - entity2->GetImage()->position.y;

        //float pushDirLength = sqrt(pushDirX * pushDirX + pushDirY * pushDirY);

        //float overlap = 25.0f; // You can adjust this value as needed

        //if (door->GetDoorClosed())
        //{
        //    // Apply the push force only if there is overlap
        //    if (pushDirLength > 0)
        //    {
        //        pushDirX /= pushDirLength;
        //        pushDirY /= pushDirLength;

        //        // Apply the push force only if there is overlap
        //        if (pushDirLength < overlap)
        //        {
        //            entity2->GetImage()->position.x -= (pushDirX * (overlap - pushDirLength)) / 3.f;
        //            entity2->GetImage()->position.y -= (pushDirY * (overlap - pushDirLength)) / 3.f;
        //        }
        //    }
        //}
    }
}
