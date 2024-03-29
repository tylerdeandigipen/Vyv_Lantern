#include "Entity.h"
#include "Transform.h"
#include "BehaviorReceiver.h"

BehaviorReceiver::BehaviorReceiver() : Behavior(Behavior::bEmitter), receiver(new CheckPoint)
{
    _type = this; // i dont know what this is
}

BehaviorReceiver::BehaviorReceiver(BehaviorReceiver const& other) : Behavior(other), receiver(other.receiver)
{
    _type = this;
    delete receiver;
    receiver = NULL;
}

BehaviorReceiver::~BehaviorReceiver()
{
    // call the function to delete mirror whenever tyler makes it ioujdfsjklsdfjkl;dfsjkl;
}

std::string BehaviorReceiver::GetName()
{
    return Name();
}

std::string BehaviorReceiver::Name()
{
    return "BehaviorReceiver";
}

void BehaviorReceiver::SetInputHandler(Inputs* _input)
{
    UNREFERENCED_PARAMETER(_input);
}

void BehaviorReceiver::Init()
{
    if (Parent())
    {
        Transform* trans = Parent()->Has(Transform);
        if (trans)
        {
            ImageBuffer* image = Parent()->GetImage();
            gfxVector2 test = image->size;
            // horizontal
            receiver->xPos1 = { trans->GetTranslation()->x, trans->GetTranslation()->y + (test.y / 2) };
            receiver->xPos2 = { trans->GetTranslation()->x + (test.x), trans->GetTranslation()->y + (test.y / 2) };
            // vertical 
            receiver->yPos1 = { trans->GetTranslation()->x + (test.x / 2), trans->GetTranslation()->y };
            receiver->yPos2 = { trans->GetTranslation()->x + (test.y / 2), trans->GetTranslation()->y + test.y };
            Renderer::GetInstance()->laserHandler.AddCheckPoint(receiver);
        }
    }
}

Behavior* BehaviorReceiver::Clone() const
{
    return new BehaviorReceiver(*this);
}

void BehaviorReceiver::Update(float dt)
{
    UNREFERENCED_PARAMETER(dt);

    if (Parent() && Parent()->Has(Transform)) {
        Vector2 position = *Parent()->Has(Transform)->GetTranslation();
        // set reflection position when the mirror stops moving or is not moving;
        //mirror->p

    }
}

void BehaviorReceiver::Read(json jsonData)
{
    Init();
    receiver->isSolid = true;
    if (jsonData["Solid"].is_boolean())
    {
        receiver->isSolid = jsonData["Solid"];
    }
    // do not know what it needs but for now it shall be callin init

}
