#include "Component.h"
#include "reciever.h"
#include "Renderer.h"


#define LargeNum 9999


Reiever::Reiever() : Component(Component::cEmitter)
{

}

Reiever::~Reiever()
{

}

Component* Reiever::Clone() const
{
	return new Reiever(*this);
}


void Reiever::Read(json jsonData)
{

}


void Reiever::Update(float dt)
{
	Endpoint_Cal();
	if (parent->CheckIfSource(*endpoint))
	{
		ifsource = true;
	}
	else
	{
		ifsource = false;
	}
	
}


void Reiever::Endpoint_Cal()
{

	Renderer* pointyBoi = Renderer::GetInstance();


	gfxVector2 compare = { 0,0 };
	//if direction then compare

	if (direction == up)
	{
		compare = pointyBoi->CheckLineForObjects(position->x, position->y,position->x,position->y + LargeNum);
	}
	else if (direction == right)
	{
		compare = pointyBoi->CheckLineForObjects(position->x, position->y, position->x + LargeNum, position->y);
	}
	else if (direction == down)
	{
		compare = pointyBoi->CheckLineForObjects(position->x, position->y, position->x, position->y - LargeNum);
	}
	else if (direction == left)
	{
		compare = pointyBoi->CheckLineForObjects(position->x, position->y, position->x - LargeNum, position->y);
	}

	endpoint = &compare;
}






void Reiever::SetPosition(Vector2 SetP)
{
	position = &SetP;
}
void Reiever::SetDirection(Directon SetP)
{
	direction = SetP;
}


