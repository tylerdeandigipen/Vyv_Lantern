//------------------------------------------------------------------------------
//
// File Name:	Collider.h
// Author(s):	Michael Howard (michael.howard)
// Purpose:		collider component
// 
// Copyright ?2023 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------
#pragma once
#include "stdafx.h"
#include "Component.h"

class Collider : public Component
{
public:
	enum ColliderType { AABB };


	_inline Collider::ColliderType type() const { return bType; };
	_inline Collider* Type() const { return _type; };

	Collider(ColliderType _type);
	Collider(Collider const& other);
	~Collider();
	std::string GetName() override { return std::string(); };
	//virtual CreateComponent(mType);

	// used to invoke the copy constructor
	Component* Clone(void) const override;

	//component-specific render code.
	void Update(float dt);
	virtual void Render() const {};
	virtual void Read(json jsonData) override;
	virtual void Init() {};

	virtual void Check(Collider* other) {};

	void SetCollisionHandler(std::function<void(Entity*, Entity*)> _handler);
	std::function<void(Entity*, Entity*)> GetCollisionHandler();


	static void PlayerCollisionHandler(Entity* thisone, Entity* other);


	Collider* _type;

private:
	ColliderType bType; 
	std::function<void(Entity*, Entity*)> handler;
};

