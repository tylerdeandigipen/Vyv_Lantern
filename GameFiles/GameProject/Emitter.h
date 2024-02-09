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

	bool IsEmittingRight() { return isEmittingRight; };
	bool IsEmittingLeft() { return isEmittingLeft; };

	void SetEmittingRight(bool state) { isEmittingRight = state; isDirty = true; }; //probably a control flag and collision flag
	void SetEmittingLeft(bool state) { isEmittingLeft = state; isDirty = true; }; //probably a control flag and collision flag



	//standard get/set funcitons
	float GetMaxEmitDistance() { return emitMaxDistance; };
	gfxVector2* GetPositionRight() { return emitPositionRight; };// start of right side collision
	gfxVector2* GetPositionLeft() { return emitPositionLeft; }; //start of left side collision
	gfxVector2* GetDirectionLeft() { return emitLTrigDir; };
	gfxVector2* GetDirectionRight() { return emitRTrigDir; };
	float GetDistance() { return emitMaxDistance; };
	void SetDrawDistance(float distance) { DrawDistance = distance; }
	void SetCollidedRight(bool state) { hasCollidedRight = state; }
	void SetCollidedLeft(bool state) { hasCollidedLeft = state; }
	bool GetCollidedRight() { return hasCollidedRight; };
	bool GetCollidedLeft() { return hasCollidedLeft; };
	void SetRightTrigger(bool state) { rightTrigger = state; };
	void SetLeftTrigger(bool state) { leftTrigger = state; };
	void SetPositionRight(float x, float y);
	void SetPositionLeft(float x, float y);
	void SetEmitPositionEndR(float x, float y);
	void SetEmitPositionEndL(float x, float y);
	gfxVector2* GetEmitPositionEndR() { return emitpositionEndR; }; // end right side collision laser
	gfxVector2* GetEmitPositionEndL() { return emitpositionEndL; }; // end left side collision laser

	static std::string Name()
	{
		return "Emitter";
	};

	std::string GetName() override
	{
		return std::string();
	};

	static void EmitterCollisionHandler(Entity& object1, Entity& object2);

private:

	gfxVector2* emitLTrigDir;
	gfxVector2* emitRTrigDir;

	gfxVector2* emitPositionRight;
	gfxVector2* emitPositionLeft;

	gfxVector2* emitpositionEndR;
	gfxVector2* emitpositionEndL;

	float emitMaxDistance;
	float DrawDistance;
	bool isDirty;
	bool hasCollidedRight;
	bool hasCollidedLeft;

	bool isEmittingRight;
	bool isEmittingLeft;
	bool rightTrigger;
	bool leftTrigger;
	int iLeftLaser;
	int iRightLaser;

};

