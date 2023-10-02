//------------------------------------------------------------------------------
//
// File Name:	Transform.h
// Author(s):	Michael Howard
// Purpose:		Class for all Entities Transform data
//
// Copyright � 2023 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------
#pragma once

#include "Component.h"
#include "Vector.h"

class Transform : public Component
{
public:
	Transform();
	Transform(Transform const& transform);
	~Transform();
	Component* Clone() const override;
	void Read(Stream stream) override;
	//const Matrix2D* GetMatrix();
	const gfxVector2* GetTranslation();
	float GetRotation();
	const gfxVector2* GetScale();
	void SetTranslation(gfxVector2* _translation);
	void SetRotation(float _rotation);
    void SetScale(const gfxVector2* _scale);
	gfxVector2* translation;
private:

	gfxVector2 scale;
	float rotation;

	bool isDirty;

	//Matrix2D matrix;
};

