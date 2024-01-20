//------------------------------------------------------------------------------
//
// File Name:	ColliderAABB.h
// Author(s):	Michael Howard (michael.howard)
// Purpose:		AABB collider for most if not all game objects
// 
// Copyright © 2023 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------
#pragma once
#include "stdafx.h"
#include "Collider.h"

class ColliderAABB : public Collider
{
public:
	ColliderAABB();
	ColliderAABB(ColliderAABB const& other);
	~ColliderAABB();
	std::string GetName() override { return std::string(); };
	//virtual CreateComponent(mType);

	// used to invoke the copy constructor
	Collider* Clone(void) const override;
	void Check(Collider* other) override;
	bool IsColliding(const Collider& other);
	static std::string Name();
	//component-specific render code.
	void Update(float dt);
	void Render() const {};
	void Read(json jsonData) override;
	void Init() {};
private:

};

