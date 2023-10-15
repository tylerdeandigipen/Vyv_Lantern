//------------------------------------------------------------------------------
//
// File Name:	Physics.h
// Author(s):	Michael Howard
// Purpose:		The physics for each entity
//
// Copyright © 2023 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------
#pragma once
#include "stdafx.h"
#include "Component.h"

class Physics : public Component
{
public:
	Physics();
	Physics(Physics const& physics);
	~Physics();
	Component* Clone() const override;
	std::string GetName() override;
	static std::string Name();
	// const Vector2D* GetOldTranslation() const 
	// const Vector2D* GetAcceleration() const;
	// const Vector2D* GetVelocity() const;
	void Read(json jsonData) override;
	void Update(float dt);
private:
	// Variables and functions will need to be custom made
	// CS230 examples are set up for a moving rocket 

};

