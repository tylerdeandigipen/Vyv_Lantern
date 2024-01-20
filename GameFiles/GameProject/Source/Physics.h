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
#include "Vector.h"

class Physics : public Component
{
public:
	Physics();
	Physics(Physics const& physics);
	~Physics();
	Component* Clone() const override;
	std::string GetName() override;
	static std::string Name();
	const gfxVector2 GetOldTranslation();
	void Read(json jsonData) override;
	void Update(float dt);
private:
	gfxVector2 oldTranslation;
};

