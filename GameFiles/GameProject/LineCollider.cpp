#include "LineCollider.h"


LineCollider::LineCollider(): Component(Component::cLineCollider)
{
	collidingWithRight = nullptr;
	collidingWithLeft = nullptr;
	position1 = new gfxVector2();
	position2 = new gfxVector2();
}

LineCollider::LineCollider(LineCollider const & cpy) : Component(Component::cLineCollider)
{
	collidingWithLeft = nullptr;
	collidingWithRight = nullptr;
	position1 = new gfxVector2(*cpy.position1);
	position2 = new gfxVector2(*cpy.position2);
}


LineCollider::~LineCollider()
{
	delete position1;
	delete position2;
}

Component* LineCollider::Clone(void) const
{
	return new LineCollider(*this);
}

//component-specific render code.
void LineCollider::Update(float dt) { 
	dt = dt; 
	//debug line draw potential

}

void LineCollider::Read(json jsonData)
{
	if (jsonData["LeftMostPoint"].is_object())
	{
		json pos = jsonData["LeftMostPoint"];
		position1->x = pos["x"];
		position1->y = pos["y"];
	}

	if (jsonData["RightMostPoint"].is_object())
	{
		json pos = jsonData["RightMostPoint"];
		position1->x = pos["x"];
		position1->y = pos["y"];
	}

}

void LineCollider::Render() const
{
	//does not render
};



