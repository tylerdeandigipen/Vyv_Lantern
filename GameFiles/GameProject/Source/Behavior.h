
//------------------------------------------------------------------------------
//
// File Name:	Behavior.h
// Author(s):	Michael Howard (michael.howard)
// Purpose:		behavior component
// 
// Copyright ?2023 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------
#pragma once

#include "Component.h"
class Inputs;

class Behavior : public Component
{
public:
	enum BehaviorType { Player, Switch, Door, Mirror };
	_inline Behavior::BehaviorType type() const { return bType; };
	inline Behavior* Type() const { return _type; };

	Behavior(BehaviorType _type);
	Behavior(Behavior const& other);

	virtual ~Behavior();
	std::string GetName() override { return std::string(); };
	void Read(json jsonData);

	virtual void SetInputHandler(Inputs* input) {};
	Component* Clone(void) const override;
	
	virtual void SetCurr(int state);
	virtual void SetNext(int state);
	virtual int GetCurr() const;
	virtual int GetNext() const;

	virtual void Init() {} ;
	void Update(float dt);
	Behavior* _type;

private:
	BehaviorType bType;
	int currState;
	int nextState;
};

