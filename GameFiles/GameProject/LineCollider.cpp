#include "LineCollider.h"


LineCollider::LineCollider(): Component(Component::cLineCollider)
{

	position1 = new gfxVector2();
	position2 = new gfxVector2();
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

}

void LineCollider::Read(json jsonData)
{
	/*
	{
	  "Type": "LineCollider",
      "LeftMostPoint": {
          "x": 0,
          "y": 0
      },       
      "RightMostPoint": {
          "x": 50,
          "y": 0
      }
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



