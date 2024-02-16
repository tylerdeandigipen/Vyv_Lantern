#include "LineCollider.h"


LineCollider::LineCollider(): Component(Component::cLineCollider)
{

	position1 = new gfxVector2();
	position2 = new gfxVector2();
	IsDirty = false;
}

LineCollider::LineCollider(LineCollider const & cpy) : Component(Component::cLineCollider)
{

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
	//if IsDirty
	// Do Stuff
}

void LineCollider::Read(json jsonData)
{
	/*
	  "Type": "LineCollider",
      "LeftMostPoint": {
          "x": 0,
          "y": 0
      },       
      "RightMostPoint": {
          "x": 50,
          "y": 0
      }

	*/

	
	if (jsonData["LeftMostPoint"].is_object())
	{
		json pos = jsonData["LeftMostPoint"];
		position1->x = pos["x"];
		position1->y = pos["y"];
	}

	if (jsonData["RightMostPoint"].is_object())
	{
		json pos = jsonData["RightMostPoint"];
		position2->x = pos["x"];
		position2->y = pos["y"];
	}

}

void LineCollider::Render() const
{
	//does not render
};


void LineCollider::SetPosition1(gfxVector2* position)
{
	position1->x = position->x;
	position1->y = position->y;
	IsDirty = true;
}

void LineCollider::SetPosition2(gfxVector2* position)
{
	position2->x = position->x;
	position2->y = position->y;
	IsDirty = true;
}
