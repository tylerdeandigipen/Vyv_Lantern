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
    UNREFERENCED_PARAMETER(_input);
}

int numEmitterParticles = 20;
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

            //spawn particles around the emitter
            for (int i = 0; i < numEmitterParticles; i++)
            {
                float angle = (rand() % 628) / 100;
                Vector2 angleVec{ cosf(angle),sinf(angle) };
                Particle* temp = new Particle{ laser->pos + Vector2{(float)(rand() % 8) - 3,(float)(rand() % 4) - 1},angleVec,Vector2{angleVec.x * ((rand() % 10) - 5) * 2.5f, angleVec.y * ((rand() % 10) - 5) * 2.5f}, Color{202,245,250,255}, Particle_Laser_Emiter, 2.0f };
                Renderer::GetInstance()->particleManager->AddParticle(temp);
            }
        }
    }
}

Behavior* BehaviorEmitter::Clone() const
{
    return new BehaviorEmitter(*this);
}

void BehaviorEmitter::Update(float dt)
{
    UNREFERENCED_PARAMETER(dt);
    if (Parent() && Parent()->Has(Transform)) {
        Vector2 position = *Parent()->Has(Transform)->GetTranslation();
        // set reflection position when the mirror stops moving or is not moving;
        //mirror->p
        laser->pos = position;
        laser->pos.x += (Parent()->GetImage()->size.x / 2);
        laser->pos.y += (Parent()->GetImage()->size.y / 2);
    }
}

void BehaviorEmitter::Read(json jsonData)
{
    Init();
    Renderer::GetInstance()->laserHandler.AddLaser(laser);
    laser->isEmiting = true;
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
    UNREFERENCED_PARAMETER(collided);
}

void BehaviorEmitter::SetDirection(gfxVector2 dir)
{
    if (laser)
        laser->dir = dir;
}
