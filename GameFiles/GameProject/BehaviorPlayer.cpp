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
	Controller();
}

void BehaviorPlayer::SetInputHandler(Inputs* _input)
{
    input = _input;
}


void BehaviorPlayer::Controller()
{
    if (input->keyPressed(SDLK_w) == true)
    {
        Parent()->Has(Transform)->translation->y -= 2;
        //pixelRenderer.AddLight(pixelRenderer.staticLightSource[0]);
        //AudioManager.PlaySFX("footsteps.ogg");
    }
    if (input->keyPressed(SDLK_s) == true)
    {
        Parent()->Has(Transform)->translation->y += 2;
        //AudioManager.PlaySFX("footsteps.ogg");
    }
    if (input->keyPressed(SDLK_d) == true)
    {
        Parent()->Has(Transform)->translation->x += 2;
        //AudioManager.PlaySFX("footsteps.ogg");
    }
    if (input->keyPressed(SDLK_a) == true)
    {
        Parent()->Has(Transform)->translation->x -= 2;
        //AudioManager.PlaySFX("footsteps.ogg");
    }
}
