#pragma once
#include "Component.h"
#include "Entity.h"
#include "Emitter.h"
#include "Collider.h"

class LineCollider : public Component
{

public:
	LineCollider();
	LineCollider(LineCollider const & cpy);
	~LineCollider();

	// used to invoke the copy constructor
	Component* Clone(void) const;

	void Read(json jsonData);
	//component-specific render code.
	void Update(float dt);
	
	void Render() const;

	static std::string Name()
	{
		return "LineCollider";
	};

	std::string GetName() override
	{
		return std::string();
	};


	gfxVector2* GetPosition1() { return position1; };
	gfxVector2* GetPosition2() { return position2; };

private:
	
	Emitter* pre;


	//load position leftpoint rightpoint
	gfxVector2* position1;
	gfxVector2* position2;

};

