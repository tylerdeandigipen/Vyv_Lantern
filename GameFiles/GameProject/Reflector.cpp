#include "Reflector.h"

Component* Reflector::Clone(void) const
{

}

//component-specific render code.
void Reflector::Update(float dt) { 
	dt = dt; 
	

}

void Reflector::Read(json jsonData)
{
 

}

void Reflector::Render() const
{
	//does not render
};


void Reflector::ReflectorCollisionHandler(Entity& obj1, Entity& obj2)
{

};

void Reflector::Read(json jsonData)
{
};
