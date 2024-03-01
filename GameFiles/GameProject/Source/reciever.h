#pragma once
#include "Component.h"
#include "Vector.h"
#include "Emitter_Container.h"

enum Directon {up, right, down, left};

class Reiever : public Component
{
public:
	Reiever();
	~Reiever();
	Component* Clone() const override;
	void Read(json jsonData) override;
	void Update(float dt);
	
	bool Check_If_Source() { return ifsource; }

	//standard get/set funcitons

	void SetPosition(Vector2 SetP);
	void SetDirection(Directon SetP);
	


	std::string GetName() override
	{
		return std::string();
	};


private:
	void Endpoint_Cal();
	
	bool ifsource;
	Vector2* position;
	Vector2* endpoint;
	Directon direction;

	EmitterContainer* parent;

};