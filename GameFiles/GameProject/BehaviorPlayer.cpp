#include "BehaviorPlayer.h"
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
   // input = new Inputs(Parent()->window);
}

void BehaviorPlayer::Update(float dt)
{
	dt = dt;
    input->handleInput();
	Controller(dt);
}

void BehaviorPlayer::SetInputHandler(Inputs* _input)
{
    input = _input;
}


void BehaviorPlayer::Controller(float dt)
{
    if (input->keyPressed(SDL_SCANCODE_W))
    {
        Parent()->Has(Transform)->translation->y -= 50 * dt;
        //pixelRenderer.AddLight(pixelRenderer.staticLightSource[0]);
        //AudioManager.PlaySFX("footsteps.ogg");
    }
    if (input->keyPressed(SDL_SCANCODE_S))
    {
        Parent()->Has(Transform)->translation->y += 50 * dt;
        //AudioManager.PlaySFX("footsteps.ogg");
    }
    if (input->keyPressed(SDL_SCANCODE_D))
    {
        Parent()->Has(Transform)->translation->x += 50 * dt;
        //AudioManager.PlaySFX("footsteps.ogg");
    }
    if (input->keyPressed(SDL_SCANCODE_A))
    {
        Parent()->Has(Transform)->translation->x -= 50 * dt;
        //AudioManager.PlaySFX("footsteps.ogg");
    }
}
