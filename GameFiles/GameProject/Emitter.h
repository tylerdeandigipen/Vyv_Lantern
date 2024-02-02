#pragma once
#include "Component.h"

class gfxVector2;

/*
*
* work to Create_dumbfire line_line collision.
//laser system list
works like a laser emmit from point go in stright line hits anything must stop.

Component emmitter
emmits a line if the line colides with any viable thing do stop and calculate

Component
Reflector reflector subtypes or flags to set. are a subset of emmitter will pseudo emmit the "reflection"
"catches laser" and emmits it will need to pass laser component

mirrors have emmitters and have behavior reflector


hit reflector. change direction 90 degrees

hit checkpoint - flips bool emmits

hit checkpoint - stops laser.

mirrors set direction laser independant (default normal)

mirro types of 2, one side reflect 2 side reflect. independant

line drawing (get information)

*/


class Emitter : public Component
{
public:
	Emitter();
	Emitter(Emitter const& emmitter2cpy);
	~Emitter();
	Component* Clone() const override;

	void Read(json jsonData) override;
	void Update(float dt);
	void Render() const;

	bool IsEmitterEmitting() { return isEmitting; };

	void SetEmitting(bool state) { isEmitting = state; }; //probably a control flag and collision flag

	std::string GetName() override
	{
		return std::string();
	};

	//standard get/set funcitons
	gfxVector2* GetPosition() { return emitPosition; };
	gfxVector2* GetDirection() { return emitLTrigDir; };
	float GetDistance() { return emitMaxDistance; };
	void SetDrawDistance(float distance) { DrawDistance = distance; }
	void SetCollided(bool state) { hasCollided = state; }
	bool GetCollided() { return hasCollided; };
	void SetRightTrigger(bool state) { rightTrigger = state; };
	void SetLeftTrigger(bool state) { leftTrigger = state; };

	std::string Name()
	{
		return "Emitter";
	};

	static void EmitterCollisionHandler(Entity& object1, Entity& object2);

private:

	gfxVector2* emitLTrigDir;
	gfxVector2* emitRTrigDir;
	gfxVector2* emitPosition;

	float emitMaxDistance;
	float DrawDistance;
	bool isDirty;
	bool hasCollided;

	bool isEmitting;
	bool rightTrigger;
	bool leftTrigger;
	bool emitterTwoIsActive;
};

