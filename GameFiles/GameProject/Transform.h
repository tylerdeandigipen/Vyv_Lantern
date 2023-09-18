//------------------------------------------------------------------------------
//
// File Name:	Transform.h
// Author(s):	Michael Howard
// Purpose:		Class for all Entities Transform data
//
// Copyright © 2023 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------
#pragma once

#include "stdafx.h"
#include "Component.h"
class Transform : public Component
{
public:
	Transform();
	Transform(Transform const& transform);
	~Transform();
	Component* Clone() const override;
	void Read(Stream stream) override;
	//const Matrix2D* GetMatrix();
	//const Vector2D* GetTranslation();
	//float GetRotation();
	//const Vector2D* GetScale();
	//void SetTranslation(const Vector2D* _translation);
	//void SetRotation(float _rotation);
	//void SetScale(const Vector2D* _scale);
private:
	// Vector2D translation;

	//float rotation

	// Vector2D scale

	// bool isDirty

	//Matrix2D matrix;
};

