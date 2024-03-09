#include "Entity.h"
#include "Transform.h"
#include "BehaviorEmitter.h"

BehaviorEmitter::BehaviorEmitter() : Behavior(Behavior::bEmitter), laser(new Laser)
{
    _type = this; // i dont know what this is
}

BehaviorEmitter::BehaviorEmitter(BehaviorEmitter const& other) : Behavior(other), laser(other.laser)
{
    _type = this;
    delete laser;
    laser = NULL;
}

BehaviorEmitter::~BehaviorEmitter()
{
    // call the function to delete mirror whenever tyler makes it ioujdfsjklsdfjkl;dfsjkl;
}

std::string BehaviorEmitter::GetName()
{
    return Name();
}

std::string BehaviorEmitter::Name()
{
    return "BehaviorEmitter";
}

void BehaviorEmitter::SetInputHandler(Inputs* _input)
{
}

void BehaviorEmitter::Init()
{
    if (Parent())
    {
        Transform* trans = Parent()->Has(Transform);
        if (trans)
        {
            laser->pos = *trans->GetTranslation();
            laser->pos.x += (Parent()->GetImage()->size.x / 2);
            laser->pos.y += (Parent()->GetImage()->size.y / 2);
        }
    }
}

Behavior* BehaviorEmitter::Clone() const
{
    return new BehaviorEmitter(*this);
}

void BehaviorEmitter::Update(float dt)
{
    if (Parent() && Parent()->Has(Transform)) {
        Vector2 position = *Parent()->Has(Transform)->GetTranslation();
        // set reflection position when the mirror stops moving or is not moving;
        //mirror->p

    }
}

void BehaviorEmitter::Read(json jsonData)
{
    Init();
    Renderer::GetInstance()->laserHandler.AddLaser(laser);
    if (jsonData["direction"].is_object())
    {
        json data = jsonData["direction"];
        gfxVector2 direct;
        direct.x = data["x"];
        direct.y = data["y"];
        laser->dir = direct;
        laser->isEmiting = true;
    }
    if (jsonData["color"].is_object())
    {
        json color = jsonData["color"];
        uint8_t r = color["red"];
        uint8_t g = color["green"];
        uint8_t b = color["blue"];
        uint8_t a = color["alpha"];
        laser->color = Color{ r, g, b, a };
    }
    else
    {
        laser->color = Color{ 230,230,230,255 };
    }

}

void BehaviorEmitter::SwitchOn(bool collided)
{

}

void BehaviorEmitter::SetDirection(gfxVector2 dir)
{
    if (laser)
        laser->dir = dir;
}
