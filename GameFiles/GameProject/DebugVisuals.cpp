//------------------------------------------------------------------------------
//
// File Name:	DebugVisuals.cpp
// Author(s):	TayLee Young
// Purpose:		Displays on screen variables passed in numerically.
//
// Copyright © 2023 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "DebugVisuals.h"
#include <string>
#include <SDL/SDL.h>

DebugVisuals::DebugVisuals(SDL_Renderer* _render, int _x, int _y, int _lineSpacing, int _fontSize) : x(_x), y(_y), lineSpacing(_lineSpacing), fontSize(_fontSize)
{
	renderer = _render;

}
void DebugVisuals::AddValue(const std::string& label, int value) 
{

}
void DebugVisuals::Draw()
{

}

void DebugVisuals::Clear()
{

}