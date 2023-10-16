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
	dt = dt;
	Controller(dt);
}

void BehaviorPlayer::SetInputHandler(Inputs* _input)
{
    input = _input;
}


void BehaviorPlayer::Controller(float dt)
{
    Transform* transform = Parent()->Has(Transform);
    gfxVector2 translation = *transform->GetTranslation();
    input = Inputs::GetInstance();
    if (input->keyPressed(SDL_SCANCODE_W))
    {
        translation.y -= 50 * dt;
        //pixelRenderer.AddLight(pixelRenderer.staticLightSource[0]);
        //AudioManager.PlaySFX("footsteps.ogg");
    }
    if (input->keyPressed(SDL_SCANCODE_S))
    {
        translation.y += 50 * dt;
        //AudioManager.PlaySFX("footsteps.ogg");
    }
    if (input->keyPressed(SDL_SCANCODE_D))
    {
        translation.x += 50 * dt;
        //AudioManager.PlaySFX("footsteps.ogg");
    }
    if (input->keyPressed(SDL_SCANCODE_A))
    {
        translation.x -= 50 * dt;
        //AudioManager.PlaySFX("footsteps.ogg");
    }
    transform->SetTranslation(translation);
}
