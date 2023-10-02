//------------------------------------------------------------------------------
//
// File Name:	Transform.cpp
// Author(s):	Michael Howard
// Purpose:		Class for all Entities Transform data
//
// Copyright © 2023 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------
#include "Transform.h"

Transform::Transform() : Component(Component::cTransform)
{
}

Transform::Transform(Transform const& trans) : Component(trans)
{
}


Transform::~Transform()
{
}


Component* Transform::Clone() const
{
	return new Transform(*this);
}

void Transform::Read(Stream stream)
{
	stream = stream;
	//if (stream)
	//{
	//	StreamReadVector2D(stream, &translation);
	//	rotation = StreamReadFloat(stream);
	//	StreamReadVector2D(stream, &scale);
	//}
}


const gfxVector2* Transform::GetTranslation() { return translation; }
float Transform::GetRotation() { return rotation; }
const gfxVector2* Transform::GetScale() { return &scale; }

void Transform::SetTranslation(gfxVector2* _translation)
{
	isDirty = true;
	translation = _translation;
}

void Transform::SetRotation(float _rotation)
{
	isDirty = true;
	rotation = _rotation;
}

void Transform::SetScale(const gfxVector2* _scale)
{
	isDirty = true;
	scale = *_scale;
}

//const Matrix2D* Transform::GetMatrix()
//{
//	if (isDirty)
//	{
//		Matrix2D mTrans = { 0 };
//		Matrix2D mScale = { 0 };
//		Matrix2D mRot = { 0 };
//		Matrix2D result = { 0 };
//
//		for (int i = 0; i < 4; i++)
//			for (int j = 0; j < 4; j++)
//			{
//				mTrans.m[i][j] = 0.0f;
//				mScale.m[i][j] = 0.0f;
//				mRot.m[i][j] = 0.0f;
//				result.m[i][j] = 0.0f;
//			}
//
//		Matrix2DTranslate(&mTrans, translation.x, translation.y);
//		Matrix2DScale(&mScale, scale.x, scale.y);
//		Matrix2DRotRad(&mRot, rotation);
//		Matrix2DConcat(&result, &mRot, &mScale);
//		Matrix2DConcat(&matrix, &mTrans, &result);
//		isDirty = false;
//	}
//
//	return &matrix;
//}