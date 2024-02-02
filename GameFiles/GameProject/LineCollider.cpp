#include "LineCollider.h"


LineCollider::LineCollider(): Component(Component::cLineCollider)
{
	collidingWith = nullptr;
	position1 = new gfxVector2();
	position2 = new gfxVector2();
}

Component* LineCollider::Clone(void) const
{

}

//component-specific render code.
void LineCollider::Update(float dt) { 
	dt = dt; 
	//debug line draw potential

}

void LineCollider::Read(json jsonData)
{
 

}

void LineCollider::Render() const
{
	//does not render
};


void LineCollider::LineColliderCollisionHandler(Entity& obj1, Entity& obj2)
{

};

void LineCollider::Read(json jsonData)
{
};
